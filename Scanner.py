import numpy as np
import cv2 as cv

#Diccionario ESTADO = {} con todas las piezas grises
estado = {
    "Down" : ["Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris"],
    "Front" : ["Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris"],
    "Right" : ["Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris"],
    "Back" : ["Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris"],
    "Left" : ["Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris"],
    "Up" : ["Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris","Gris"]
}

#Convencion de colores y caras
convencion = { "Blanco" : 'D', "Azul" : 'F', "Rojo" : 'R', "Verde" : 'B', "Naranja" : 'L', "Amarillo" : 'U'}

#Diccionario de colores enteros
coloresInt = {
    1: "Blanco",
    2: "Azul",
    3: "Rojo",
    4: "Verde",
    5: "Naranja",
    6: "Amarillo"}

#Valores BGR para dibujar figuras
coloresBase = {
    "Blanco" : (255,255,255),
    "Azul" : (168,70,0),
    "Rojo" : (0,0,255),
    "Verde" : (19,156,62),
    "Naranja" : (0,128,255),
    "Amarillo" : (1,190,229),
    "Negro" : (0,0,0),
    "Gris" : (155,155,155)
}

#--------------Coordenadas para dibujar las caras que han sido escaneadas 
coordenadasEscaneados = {
    "Down" : [
    [160,310], [205,310], [250,310],
    [160,355], [205,355], [250,355],
    [160,400], [205,400], [250,400]],
    "Front" : [
    [160,160], [205,160], [250,160],
    [160,205], [205,205], [250,205],
    [160,250], [205,250], [250,250]],
    "Right" : [
    [310,160], [355,160], [400,160],
    [310,205], [355,205], [400,205],
    [310,250], [355,250], [400,250]],
    "Back" : [
    [460,160], [505,160], [550,160],
    [460,205], [505,205], [550,205],
    [460,250], [505,250], [550,250]],
    "Left" : [
    [10,160], [55,160], [100,160],
    [10,205], [55,205], [100,205],
    [10,250], [55,250], [100,250]],
    "Up" : [
    [160,10], [205,10], [250,10],
    [160,55], [205,55], [250,55],
    [160,100], [205,100], [250,100]]
}

#Duplicados (Omptimizar luego)
downEscaneado = [[160,310], [205,310], [250,310],[160,355], [205,355], [250,355], [160,400], [205,400], [250,400]]
frontEscaneado  = [[160,160], [205,160], [250,160],[160,205], [205,205], [250,205],[160,250], [205,250], [250,250]]
rightEscaneado = [[310,160], [355,160], [400,160],[310,205], [355,205], [400,205],[310,250], [355,250], [400,250]]
backEscaneado = [[460,160], [505,160], [550,160],[460,205], [505,205], [550,205],[460,250], [505,250], [550,250]]
leftEscaneado = [[10,160], [55,160], [100,160],[10,205], [55,205], [100,205],[10,250], [55,250], [100,250]]
upEscaneado = [[160,10], [205,10], [250,10],[160,55], [205,55], [250,55],[160,100], [205,100], [250,100]]
#--------------Coordenadas para dibujar las caras que han sido escaneadas 



#-------------Coodernadas de los ROI 30x30----------------------------#
rois = [
    [200,120], [300,120], [400,120],
    [200,220], [300,220], [400,220],
    [200,320], [300,320], [400,320]
]

#-------------Coordenadas de la vista actual 30x30----------------------------#
vistaActual = [
    [20, 20], [54, 20], [88, 20],
    [20, 54], [54, 54], [88, 54],
    [20, 88], [54, 88], [88, 88]
]

#------------Coordenadas de las letras----------------------------#
letraD = [215,385] 
letraF = [215,235]
letraR = [365,235]
letraB = [515,235]
letraL = [65,235]
letraU = [215,85]

#----------------Coordenadas de botones----------------------------#
botonAlto = 50
botonAncho = 250

botonD = [700,90]
botonF = [700,155]
botonR = [700,220]
botonB = [700,285]
botonL = [700,350]
botonU = [700,415]

botonTerminar = [540,550]
botonReiniciar = [800,550]

#Fuente del texto
fuente = cv.FONT_HERSHEY_COMPLEX

#Iniciar y configurar webcam
cap = cv.VideoCapture(1)
cap.set(3, 1080)
cap.set(4, 720)

#----Funcion para detectar color en un pixel
def deteccionColor (h, s, v):

    if h >= 0 and h <= 179 and s >= 0 and s <= 55 and v >= 95 and v <= 255:
        return "Blanco"

    if h >= 0 and h < 5:
        return "Rojo"
    
    elif h > 5 and h <= 13:
        return "Naranja"
    
    elif h > 13 and h <= 30 :
        return "Amarillo"

    elif h > 31 and h <= 85 :
        return "Verde"

    elif h > 85 and h <= 114 :
        return "Azul"
    
    

    return "Negro"
    
#------------------------------Dibujar ROIs en frame principal-------------------------------#
def dibujarROI (frame):

    for x, y in rois:
        cv.rectangle(frame, (x,y), (x+30, y+30), (255,255,255), 2)

#--------------------------------------------------------------------------------------------#
#------------------------------Dibujar Vista actuall-------------------------------#
def dibujarVistaActual (frame):

    for x, y in vistaActual:
        cv.rectangle(frame, (x,y), (x+30, y+30), (255,255,255), 2)

#--------------------------------------------------------------------------------------------#

#----------------------------Dibujar botones de escaneo------------------------------------#
def dibujarBotones(frame):
    
    cv.rectangle(frame, botonD, (botonD[0] + botonAncho, botonD[1] + botonAlto), coloresBase["Blanco"], -1)
    cv.rectangle(frame, botonF, (botonF[0] + botonAncho, botonF[1] + botonAlto), coloresBase["Azul"], -1)
    cv.rectangle(frame, botonR, (botonR[0] + botonAncho, botonR[1] + botonAlto), coloresBase["Rojo"], -1)
    cv.rectangle(frame, botonB, (botonB[0] + botonAncho, botonB[1] + botonAlto), coloresBase["Verde"], -1)
    cv.rectangle(frame, botonL, (botonL[0] + botonAncho, botonL[1] + botonAlto), coloresBase["Naranja"], -1)
    cv.rectangle(frame, botonU, (botonU[0] + botonAncho, botonU[1] + botonAlto), coloresBase["Amarillo"], -1)
    cv.rectangle(frame, botonTerminar, (botonTerminar[0] + botonAncho, botonTerminar[1] + botonAlto), coloresBase["Verde"], -1)
    cv.rectangle(frame, botonReiniciar, (botonReiniciar[0] + botonAncho, botonReiniciar[1] + botonAlto), coloresBase["Rojo"], -1)

    cv.putText(frame, "Inferior (Blanca)", (botonD[0] + 35, botonD[1] + 32), fuente, 0.65, coloresBase["Negro"],2)
    cv.putText(frame, "Frontal (Azul)", (botonF[0] + 45, botonF[1] + 32), fuente, 0.65, coloresBase["Blanco"], 2)
    cv.putText(frame, "Derecha (Roja)", (botonR[0] + 40, botonR[1] + 32), fuente, 0.65, coloresBase["Blanco"], 2)
    cv.putText(frame, "Trasera (Verde)", (botonB[0] + 38, botonB[1] + 32), fuente, 0.65, coloresBase["Blanco"], 2)
    cv.putText(frame, "Izquierda (Naranja)", (botonL[0] + 18, botonL[1] + 32), fuente, 0.65, coloresBase["Blanco"], 2)
    cv.putText(frame, "Superior (Amarilla)", (botonU[0] + 18, botonU[1] + 32), fuente, 0.65, coloresBase["Blanco"], 2)
    cv.putText(frame, "Terminar", (botonTerminar[0] + 45, botonTerminar[1] + 32), fuente, 0.65, coloresBase["Blanco"], 2)
    cv.putText(frame, "Reiniciar", (botonReiniciar[0] + 45, botonReiniciar[1] + 32), fuente, 0.65, coloresBase["Blanco"], 2)

#------------------------------------Dibujar Escaneados en tiempo real----------------------------------------# *************************************************PROBANDO
def dibujarEscaneados (frame, coordenadasEscaneados, estado):

    for lado in estado:
        posicion = 0
        for x,y in coordenadasEscaneados[lado]:
            #Recorre los 9 cuadros de cada LADO
            couleur = estado[lado][posicion]
            cv.rectangle(frame, (x,y), (x+40, y+40), coloresBase[couleur], -1)
            posicion +=1

#-------------------------------------------------------------------------------------------------------------#


#----------------------------Dibujar texto en escaneos guardados---------------------------#
def dibujarTexto(frame):
    
    cv.putText(frame, 'D', letraD, fuente, 1, (0,0,0), 3)
    cv.putText(frame, 'F', letraF, fuente, 1, (0,0,0), 3)
    cv.putText(frame, 'R', letraR, fuente, 1, (0,0,0), 3)
    cv.putText(frame, 'B', letraB, fuente, 1, (0,0,0), 3)
    cv.putText(frame, 'L', letraL, fuente, 1, (0,0,0), 3)
    cv.putText(frame, 'U', letraU, fuente, 1, (0,0,0), 3)

#------------------------------------------------------------------------------------------#

isDownScanned = False
isFrontScanned = False
isRightScanned = False
isBackScanned = False
isLeftScanned = False
isUpScanned = False



#-----------------------------------------------------Listeners para botones en ventana principal-----------------------------------------------------#
def botones_listener (event, x, y, flags, param):

    if event == cv.EVENT_LBUTTONDOWN:

        #-------------------------------------------------ESCANEAR DOWN (BLANCO)#-------------------------------------------------
        if x >= botonD[0] and x <= botonD[0] + botonAncho and y >= botonD[1] and y <= botonD[1] + botonAlto:

            #Usa la variable GLOBAL
            global isDownScanned
            
            #Si la cara ya se escaneo, salir de la funcion
            if isDownScanned:
                print("La cara inferior ya fue escaneada")
                return
            
            #Si no se ha escaneado, validar y llenar
            else:
                #Tomar el pixel del centro
                pixel = imagen[57,57]
                
                #Es la cara correcta?
                if (pixel[0] == 255) and (pixel[1] == 255) and (pixel[2] == 255):
                    print("Si hay un centro blanco")
                    print("Cara inferior escaneada correctamente")
                    isDownScanned = True

                    colores = [] #Lista para almacenar los colores 

                    #Recorrer vistaActual
                    for x, y in vistaActual:
                        pixel = imagen[y+5,x+5]
                        if (pixel[0] == 255) and (pixel[1] == 255) and (pixel[2] == 255):
                            colores.append("Blanco")
                        if (pixel[0] == 168) and (pixel[1] == 70) and (pixel[2] == 0):
                            colores.append("Azul")
                        if (pixel[0] == 0) and (pixel[1] == 0) and (pixel[2] == 255):
                            colores.append("Rojo")
                        if (pixel[0] == 19) and (pixel[1] == 156) and (pixel[2] == 62):
                            colores.append("Verde")
                        if (pixel[0] == 0) and (pixel[1] == 128) and (pixel[2] == 255):
                            colores.append("Naranja")
                        if (pixel[0] == 1) and (pixel[1] == 190) and (pixel[2] == 229):
                            colores.append("Amarillo")

                    #PRUEBA DE IMPRESION
                    actual = 0
                    for x, y in downEscaneado:
                        print(x, y,coloresBase[colores[actual]])
                        actual+=1

                    estado["Down"] = colores
    
                #No es la cara correcta
                else:
                    print("No hay un centro blanco, intente otra vez")
                    #Mostrar mensaje error TODO
                    return        
        #---------------------------------------------------------------------------------------------------------------------------#       
                


        #-------------------------------------------------ESCANEAR FRONT (AZUL)-----------------------------------------------------#     
        if x >= botonF[0] and x <= botonF[0] + botonAncho and y >= botonF[1] and y <= botonF[1] + botonAlto:

            #Usa la variable GLOBAL
            global isFrontScanned
            
            #Si la cara ya se escaneo, salir de la funcion
            if isFrontScanned:
                print("La cara frontal ya fue escaneada")
                return
            
            #Si no se ha escaneado, validar y llenar
            else:
                #Tomar el pixel del centro
                pixel = imagen[57,57]

                #Es la cara correcta?
                if (pixel[0] == 168) and (pixel[1] == 70) and (pixel[2] == 0):
                    print("Si hay un centro azul")
                    print("Cara frontal escaneada correctamente")
                    isFrontScanned = True

                    colores = [] #Lista para almacenar los colores 

                    #Recorrer vistaActual
                    for x, y in vistaActual:
                        pixel = imagen[y+5,x+5]
                        if (pixel[0] == 255) and (pixel[1] == 255) and (pixel[2] == 255):
                            colores.append("Blanco")
                        if (pixel[0] == 168) and (pixel[1] == 70) and (pixel[2] == 0):
                            colores.append("Azul")
                        if (pixel[0] == 0) and (pixel[1] == 0) and (pixel[2] == 255):
                            colores.append("Rojo")
                        if (pixel[0] == 19) and (pixel[1] == 156) and (pixel[2] == 62):
                            colores.append("Verde")
                        if (pixel[0] == 0) and (pixel[1] == 128) and (pixel[2] == 255):
                            colores.append("Naranja")
                        if (pixel[0] == 1) and (pixel[1] == 190) and (pixel[2] == 229):
                            colores.append("Amarillo")

                    #PRUEBA DE IMPRESION
                    actual = 0
                    for x, y in frontEscaneado:
                        print(x, y,coloresBase[colores[actual]])
                        actual+=1

                    estado["Front"] = colores

                #No es la cara correcta
                else:
                    print("No hay un centro azul, intente otra vez")
                    #Mostrar mensaje error TODO
                    return
        #---------------------------------------------------------------------------------------------------------------------------#




        #-------------------------------------------------ESCANEAR RIGHT (ROJO)-----------------------------------------------------#  
        if x >= botonR[0] and x <= botonR[0] + botonAncho and y >= botonR[1] and y <= botonR[1] + botonAlto:

            #Usa la variable GLOBAL
            global isRightScanned
            
            #Si la cara ya se escaneo, salir de la funcion
            if isRightScanned:
                print("La cara derecha ya fue escaneada")
                return
            
            #Si no se ha escaneado, validar y llenar
            else:
                #Tomar el pixel del centro
                pixel = imagen[57,57]

                #Es la cara correcta?
                if (pixel[0] == 0) and (pixel[1] == 0) and (pixel[2] == 255):
                    print("Si hay un centro rojo")
                    print("Cara derecha escaneada correctamente")
                    isRightScanned = True

                    colores = [] #Lista para almacenar los colores 

                    #Recorrer vistaActual
                    for x, y in vistaActual:
                        pixel = imagen[y+5,x+5]
                        if (pixel[0] == 255) and (pixel[1] == 255) and (pixel[2] == 255):
                            colores.append("Blanco")
                        if (pixel[0] == 168) and (pixel[1] == 70) and (pixel[2] == 0):
                            colores.append("Azul")
                        if (pixel[0] == 0) and (pixel[1] == 0) and (pixel[2] == 255):
                            colores.append("Rojo")
                        if (pixel[0] == 19) and (pixel[1] == 156) and (pixel[2] == 62):
                            colores.append("Verde")
                        if (pixel[0] == 0) and (pixel[1] == 128) and (pixel[2] == 255):
                            colores.append("Naranja")
                        if (pixel[0] == 1) and (pixel[1] == 190) and (pixel[2] == 229):
                            colores.append("Amarillo")

                    #PRUEBA DE IMPRESION
                    actual = 0
                    for x, y in rightEscaneado:
                        print(x, y,coloresBase[colores[actual]])
                        actual+=1

                    estado["Right"] = colores

                #No es la cara correcta
                else:
                    print("No hay un centro rojo, intente otra vez")
                    #Mostrar mensaje error TODO
                    return
        #---------------------------------------------------------------------------------------------------------------------------#

        if x >= botonB[0] and x <= botonB[0] + botonAncho and y >= botonB[1] and y <= botonB[1] + botonAlto:
            print ("Boton D")

        if x >= botonL[0] and x <= botonL[0] + botonAncho and y >= botonL[1] and y <= botonL[1] + botonAlto:
            print ("Boton L")

        if x >= botonU[0] and x <= botonU[0] + botonAncho and y >= botonU[1] and y <= botonU[1] + botonAlto:
            print ("Boton U")

        if x >= botonTerminar[0] and x <= botonTerminar[0] + botonAncho and y >= botonTerminar[1] and y <= botonTerminar[1] + botonAlto:
            print ("Boton Terminar")

        if x >= botonReiniciar[0] and x <= botonReiniciar[0] + botonAncho and y >= botonReiniciar[1] and y <= botonReiniciar[1] + botonAlto:
            print ("Boton Reiniciar")

#-----------------------------------------------------------------------------------------------------------------------------------------------------#





#Matriz de 0's para dibujar Escaneos Guardados
escaneos = np.zeros((500,700,3), dtype='uint8')

while True:
    

    #Valores HSV de ROIS
    hsv = []
    ret, imagen = cap.read()
    frame = cv.cvtColor(imagen, cv.COLOR_BGR2HSV)

    #----------Dibujar ROI y Vista Previa
    #Ventana de escaneo
    dibujarROI(imagen)
    dibujarVistaActual(imagen)
    dibujarBotones(imagen)

    #Vetana de estado del cubo
    dibujarEscaneados (escaneos, coordenadasEscaneados, estado)
    dibujarTexto(escaneos)

    #Guardar valores HSV
    for i in range(9):
        hsv.append(frame[rois[i][1]+10][rois[i][0]+10])

    #Actualizar Vista previa en tiempo real
    a = 0
    for x,y in vistaActual:
        colorNombre = deteccionColor(hsv[a][0],hsv[a][1],hsv[a][2])
        cv.rectangle(imagen, (x,y), (x+30,y+30), coloresBase[colorNombre], -1)
        a+=1

    #Mostrar ventanas y agregar listeners
    cv.imshow ("Escanear",imagen)
    cv.imshow("Escaneos", escaneos)
    cv.setMouseCallback ("Escanear", botones_listener, param=imagen)

    #Condicion de salida
    if cv.waitKey(20) & 0xFF == ord('q'):
        break