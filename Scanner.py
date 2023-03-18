import numpy as np
import cv2 as cv

#Diccionario ESTADO = {} con todas las piezas blancas

#Diccionario de colores enteros
coloresInt = {
    1: "Blanco",
    1: "Blanco",
    1: "Blanco",
    1: "Verde",
    1: "Naranja",
    1: "Amarillo"}

#Valores BGR para dibujar figuras
coloresBase = {
    "Blanco" : (255,255,255),
    "Azul" : (255,0,0),
    "Rojo" : (0,0,255),
    "Verde" : (0,255,0),
    "Naranja" : (0,128,255),
    "Amarillo" : (0,255,255),
    "Negro" : (0,0,0) #Testing purposes only ***
}

#Coodernadas de los ROI 30x30
rois = [
    [200,120], [300,120], [400,120],
    [200,220], [300,220], [400,220],
    [200,320], [300,320], [400,320]
]

#Coordenadas de la vista actual 30x30
vistaActual = [
    [20, 20], [54, 20], [88, 20],
    [20, 54], [54, 54], [88, 54],
    [20, 88], [54, 88], [88, 88]
]

#Iniciar webcam
cap = cv.VideoCapture(1)

#Funcion para detectar color en un pixel
def deteccionColor (h, s, v):

    if h >= 0 and h < 8:
        return "Rojo"
    
    elif h > 8 and h <= 12:
        return "Naranja"
    
    elif h > 15 and h <= 60 and s >= 20 and s <= 220 and v >= 80 and v <= 255:
        return "Amarillo"

    elif h >= 40 and h <= 90 and s >= 65 and s <= 255 and v >= 70 and v <= 228:
        return "Verde"

    elif h >= 70 and h <= 179 and s >= 0 and s <= 129 and v >= 90 and v <= 255:
        return "Blanco"
    
    elif h >= 90 and h <= 125 and s >= 168 and s <= 255 and v >= 45 and v <= 255:
        return "Azul"


    return "Negro"
    

#------------------------------Dibujar ROIs en frame principal-------------------------------#
def dibujarROI (frame):

    for x, y in rois:
        cv.rectangle(frame, (x,y), (x+30, y+30), (255,255,255), 2)

#--------------------------------------------------------------------------------------------#
#------------------------------Dibujar ROIs en frame principal-------------------------------#
def dibujarVistaActual (frame):

    for x, y in vistaActual:
        cv.rectangle(frame, (x,y), (x+30, y+30), (255,255,255), 2)

#--------------------------------------------------------------------------------------------#

while True:

    #Valores HSV de ROIS
    hsv = []

    ret, imagen = cap.read()
    frame = cv.cvtColor(imagen, cv.COLOR_BGR2HSV)

    #Dibujar ROI y Vista Previa
    dibujarROI(imagen)
    dibujarVistaActual(imagen)

    #Guardar valores HSV
    for i in range(9):
        hsv.append(frame[rois[i][1]+10][rois[i][0]+10])

    #Actualizar Vista previa en tiempo real
    a = 0
    for x,y in vistaActual:
        colorNombre = deteccionColor(hsv[a][0],hsv[a][1],hsv[a][2])
        cv.rectangle(imagen, (x,y), (x+30,y+30), coloresBase[colorNombre], -1)
        a+=1

    #Mostrar webcam BORRAR LUEGO
    cv.imshow ("Escaneo",imagen)
    #Condicion de salida
    if cv.waitKey(20) & 0xFF == ord('q'):
        break