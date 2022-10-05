//Algoritmo a imitar: Fridrich Reducido
/*
    Nombre: C�sar Soltero P�rez
    Fecha: 28/09/2022

    Update: 4/10/2022

    Cambios a hacer:
    -Funci�n F no es compatible con Arduino, cambiar cuando se exporta
    -PrintFace NO ser� exportada a Arduino, s�lo es una funci�n auxiliar
    -Validad colores (9 colores m�ximo de cada uno)
    -Funci�n para cancelar escaner

    Objetivos:
    -int getEdgePosition(int arista);

*/

#include <stdio.h>
#include <stdlib.h>

//Arrglos
int down[3][3] = {{0,0,0,},{0,0,0,},{0,0,0}};
int front[3][3] = {{0,0,0,},{0,0,0,},{0,0,0}};
int right[3][3] = {{0,0,0,},{0,0,0,},{0,0,0}};
int left[3][3] = {{0,0,0,},{0,0,0,},{0,0,0}};
int back[3][3] = {{0,0,0,},{0,0,0,},{0,0,0}};
int up[3][3] = {{0,0,0,},{0,0,0,},{0,0,0}};

//Declaracion de funciones
void scanFace(int arr[3][3]); //Input (En arduino son los botones)
void printFace(int arr[3][3]); //Funcion auxiliar

//Movimientos del cubo (y actualizacion)
void R(); //Done
void RP(); //Done
void L(); //Done
void LP(); //Done
void F(); //Done
void FP(); //Done
void B(); //Done
void BP(); //Done
void U(); //Done
void UP(); //Done
void D(); //Done
void DP(); //Done

//Fridrich
void cross();
void F2L(); //In progress...
void OLL(); //Done
void PLL(); //Done

//PLL
void PLLU1(); //Aristas antihorario
void PLLU2(); //Aristas horario
void PLLZ(); //Aristas adyacentes
void PLLH(); //Aristas opuestas (en cruz)
void PLLSKIP(); //Reduce PLL a 4 algoritmmos
int PLLCHECK(); //Revisa la primera fila por igualdad, regresa 0 si no hay igualdad, regresa 1-6 seg�n el numero de cara
int isOpposite(int a, int b); //Determina si dos colores son opuestos, regresa 0 si FALSO, 1 si VERDADERO
int isCubeDone(); //Inspecciona el cubo, dice si est� terminado

//OLL
void OLLDOT(); //Algoritmo para convertir el punto en cruz
void OLLLINE(); //Algoritmo para convertir la linea en cruz
void OLLL(); //Algoritmo para convertir la L en cruz
void OLLA1(); //Cruz f�cil
void OLLA2(); //Cruz (nave)
void OLLA3(); //Pez 1
void OLLA4(); //Pez 2
void OLLA5(); //Nave
void OLLA6(); //Nave 2
void OLLA7(); //Mo�o
int isCrossDone(); //Inspecciona la cara superior y regresa 0 si NO hay cruz, regresa 1 si la hay
int isL(); //Regresa 1 si hay una L (antes de la cruz)
int isDot(); //Regresa 1 si s�lo hay un punto (antes de la cruz)
int isLine(); //Regresa 1 si hay una l�nea (antes de cruz)
int isLastLayerDone(); //Regresa 1 si la cara superior ya se ha resuelto

//F2L
int getCornerPosition(int corner); //Duelve la posici�n (1-20) de la esquina down[][] que se quiere acomodar
int getEdgePosition(int arista); //Duelve la posici�n (1-20) de la arista que se quiere encontrar
void setBlock1(); //Orienta bloque 1 (esquina y arista)
void setBlock2(); //Orienta bloque 2 (esquina y arista)
void setBlock1(); //Orienta bloque 3 (esquina y arista)
void setBlock2(); //Orienta bloque 4 (esquina y arista)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////                                                    Main                                           ///////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    //Scanning Fase
    printf("Down\n");
    scanFace(down);

    printf("Front\n");
    scanFace(front);

    printf("Right\n");
    scanFace(right);

    printf("Back\n");
    scanFace(back);

    printf("Left\n");
    scanFace(left);

    printf("Up\n");
    scanFace(up);

    //Solving phase
    setBlock1();

    //Printing

    printf("\n\nFront: ");
    printFace(front);
    printf("\n\nRight: ");
    printFace(right);
    printf("\n\nBack: ");
    printFace(back);
    printf("\n\nLeft: ");
    printFace(left);
    printf("\n\nDown: ");
    printFace(down);
    printf("\n\nUp: ");
    printFace(up);


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////                                         Cross, F2L, OLL, PLL                                  ///////////////////////////////////////////////////////////////////////////////////////////////////////+
//PLL
void PLL()
{
    //El cubo entra con la cara superior ya resuelta pero con esquinas y aristas desorientados

    //Mientras el cubo NO est� resuelto
    while (isCubeDone() == 0)
    {
        //Buscar coincidencias de color en esquinas
        if (front[0][0] == front[0][2] && right[0][0] == right[0][2] && back[0][0] == back[0][2] && left[0][0] == left[0][2])
        {
            //Si TODAS las esquinas coinciden, U1, U2, Z, H

            //El IF verifica Si hay una l�nea completa, U1, U2.
            if(PLLCHECK() > 0)
            {
                //El switch determina en qu� cara se encuentra la l�nea de U1 o U2 y la orienta a la cara frontal
                switch(PLLCHECK())
                {
                case 2:
                    break;

                case 3:
                    U();
                    break;

                case 4:
                    U(); U();
                    break;

                case 5:
                    UP();
                    break;

                case 6:
                    while (isCubeDone() == 0)
                    {
                        U();
                    }
                    return;
                }

                //En este punto s�lo debe decidirse si es U1 o U2
                if (isOpposite(front[0][1],right[0][1]) == 1)
                    PLLU1();

                else
                    PLLU2();

            }

            //Si no hay una l�nea completa, solo puede ser Z o H
            else
            {
                //H
                if(front[0][0] == back[0][1])
                {
                    PLLH();
                    //Girar U hasta que el cubo est� resuelto
                    while (isCubeDone() == 0)
                    {
                        U();
                    }
                }
                else
                {
                    //Acomodar Aristar Adyacentes
                    while (front[0][0] != left[0][1])
                    {
                        U();
                    }
                    PLLZ();

                }
            }

        }

        else if (front[0][0] == front[0][2] || right[0][0] == right[0][2] || back[0][0] == back[0][2] || left[0][0] == left[0][2])
        {
            //Por lo menos hay UNA coincidencia. Determinar en qu� cara se encuentra la coincidencia
            if (front[0][0] == front[0][2])
            {
                PLLSKIP();
            }

            else if (right[0][0] == right[0][2])
            {
                U();
                PLLSKIP();
            }

            else if (back[0][0] == back[0][2])
            {
                U();
                U();
                PLLSKIP();
            }

            else if(left[0][0] == left[0][2])
            {
                UP();
                PLLSKIP();
            }
        }

        else
        {
            //Si no hay NINGUNA coincidencia, hacer PLLSKIP en cualquier cara
            PLLSKIP();
            U();
        }
    }
}

//OLL
void OLL()
{
    //El cubo entra con F2L hecho

    //Hay cruz?
    while (isCrossDone() == 0)
    {
        //No. Entonces:
        //�Qu� figura tengo?
        //Orientar figura
        //Hacer cruz

        if (isLine() == 1)
        {
            //Girar U hasta orientar la l�nea en horizontal
            while (up[0][1] == up[1][1])
            {
                U();
            }
            OLLLINE();
        }
        else if (isL() == 1)
        {
            while (up[1][2] == up[1][1] || up[2][1] == up[1][1])
            {
                U();
            }
            OLLL();
        }
        else if (isDot() == 1)
        {
            OLLDOT();
        }
    }

    int color = up[1][1];
    while (isLastLayerDone() == 0)
    {
        //Se parte del hecho de que la cruz ya est� hecha, por lo tanto, (0,1)(1,0)(1,2)(2,1) ya estan cubiertos
        //con el color central, solo se necesitan verificar esquinas restantes

        if (front[0][0] == color && front[0][2] == color && back[0][0] == color && back[0][2] == color)
        {
            OLLA1(); //Cruz f�cil
        }
        else if (front[0][2] == color && back[0][0] == color && left[0][0] == color && left[0][2] == color)
        {
            OLLA2(); //Cruz 2
        }
        else if (front[0][0] == color && back[0][0] == color && left[0][0] == color)
        {
            OLLA3(); //Pez 1 (Derecha)
        }
        else if (front[0][2] == color && right[0][2] == color && back[0][2] == color)
        {
            OLLA4(); //Pez 2 (Izquierda)
        }
        else if (back[0][0] == color && back[0][2] == color)
        {
            OLLA5();
        }
        else if (front[0][0] == color && back[0][2] == color)
        {
            OLLA6();
        }
        else if (back[0][0] == color && left[0][2] == color)
        {
            OLLA7();
        }
        else
            //Si ning�n caso est� orientado, girar U
            U();
    }
    //Orientar cruz
    //Resovler cruz
}

//F2L
void F2L()
{
    //Mientras F2L No est� resuelto (while), hacer F2L
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////                                              Funciones auxiliares                                ///////////////////////////////////////////////////////////////////////////////////////////////
void scanFace(int arr[3][3])
{
    printf("Ingrese los valores de la matriz: \n");
    int num = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("Valor: ");
            scanf("%i",&num);
            arr[i][j] = num;
        }
        printf("\n");
    }
}

void printFace(int arr[3][3])
{
    printf("\nLos valores de la matriz son: \n");

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%i ",arr[i][j]);
        }
        printf("\n");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////                                              Movimientos de caras                                 //////////////////////////////////////////////////////////////////////////////////////////////
void R()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento Lineal de caras
    for (int i = 0; i < 3; i++)
    {
        aux[i] = front[i][2]; // Aux Toma valor de Front (A)
        aux2[i] = up[i][2]; // Aux2 Toma valor de Up (B)
        front[i][2] = down[i][2]; // Front (A) toma el valor de Down (D)
        up[i][2] = aux[i]; // Up (B) toma el valor de Front (A (Aux))
        aux[i] = back[i][0]; //Aux toma el valor de Back(C)
    }

    for (int i = 0; i < 3; i++)
    {
        back[i][0] = aux2[2-i]; //Back (C) el valor de Up(B(Aux2))
        down[i][2] = aux[2-i]; //Down(D) toma el valor de Back(C (aux))
    }

    //Movimiento circular de cara afectada
    for (int i = 0; i < 3; i++)
    {
        aux[i] = right[0][i];
        aux2[i] = right[i][2];
        aux3[i] = right[2][i]; ////Guarda la informaci�n de la cara afectada para girarla sobre su propio eje
        aux4[i] = right[i][0];
    }

    for (int i = 0; i < 3; i++)
    {
        right[i][2] = aux[i];
        right[2][i] = aux2[2-i];
        right[i][0] = aux3[i];
        right[0][i] = aux4[2-i];
    }

    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);
}

void RP()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento lineal
    for (int i = 0; i < 3; i++)
    {
        aux[i] = front[i][2];
        aux2[i] = down[i][2];
        aux3[i] = back[i][0];
        aux4[i] = up[i][2];
        down[i][2] = aux[i];
        front[i][2] = aux4[i];
    }

    for (int i = 0; i < 3; i++)
    {
        back[i][0] = aux2[2-i];
        up[i][2] = aux3[2-i];
    }

    //Movimiento Circular
    for (int i = 0; i < 3; i++)
    {
        aux[i] = right[0][i];
        aux2[i] = right[i][0];
        aux3[i] = right[2][i];
        aux4[i] = right[i][2];
    }

    for (int i = 0; i < 3; i++)
    {
        right[0][i] = aux4[i];
        right[i][0] = aux[2-i];
        right[2][i] = aux2[i];
        right[i][2] = aux3[2-i];
    }

    //Liberar memoria
    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);
}

void L()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento lineal
    for (int i = 0; i < 3; i++)
    {
        aux[i] = front[i][0];
        aux2[i] = down[i][0];
        aux3[i] = back[i][2];
        aux4[i] = up[i][0];
        down[i][0] = aux[i];
        front[i][0] = aux4[i];
    }

    for (int i = 0; i < 3; i++)
    {
        back[i][2] = aux2[2-i];
        up[i][0] = aux3[2-i];
    }

    //Movimiento Circular
    for (int i = 0; i < 3; i++)
    {
        aux[i] = left[0][i];
        aux2[i] = left[i][2];
        aux3[i] = left[2][i];
        aux4[i] = left[i][0];
    }

    for (int i = 0; i < 3; i++)
    {
        left[0][i] = aux4[2-i];
        left[i][2] = aux[i];
        left[2][i] = aux2[2-i];
        left[i][0] = aux3[i];
    }

    //Liberar memoria
    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);
}

void LP()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento Lineal
    for (int i = 0; i < 3; i++)
    {
        aux[i] = front[i][0];
        aux2[i] = up[i][0];
        aux3[i] = back[i][2];
        aux4[i] = down[i][0];
        front[i][0] = aux4[i];
        up[i][0] = aux[i];
    }

    for (int i = 0; i < 3; i++)
    {
        back[i][2] = aux2[2-i];
        down[i][0] = aux3[2-i];
    }

    //Movimiento circuilar
    for (int i = 0; i < 3; i++)
    {
        aux[i] = left[0][i];
        aux2[i] = left[i][0];
        aux3[i] = left[2][i];
        aux4[i] =  left[i][2];
    }

    for (int i = 0; i < 3; i++)
    {
        left[0][i] = aux4[i];
        left[i][0] = aux[2-i];
        left[2][i] = aux2[i];
        left[i][2] = aux3[2-i];
    }

    //Liberar memoria
    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);

}

void F()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento Lineal
    for (int i = 0; i < 3; i++)
    {
        aux[i] = right[i][0];
        aux2[i] = down[0][i];
        aux3[i] = left[i][2];
        aux4[i] = up[2][i];
        right[i][0] = aux4[i];
        left[i][2] = aux2[i];
    }

    for (int i = 0; i < 3; i++)
    {
        down[0][i] = aux[2-i];
        up[2][i] = aux3[2-i];
    }

    //Movimiento circular
    for (int i = 0; i < 3; i++)
    {
        aux[i] = front[0][i];
        aux2[i] = front[i][2];
        aux3[i] = front[2][i];
        aux4[i] = front[i][0];
    }

    for (int i = 0; i < 3; i++)
    {
        front[0][i] = aux4[2-i];
        front[i][2] = aux[i];
        front[2][i] = aux2[2-i];
        front[i][0] = aux3[i];
    }

    //Liberar memoria
    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);

}

void FP()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento Lineal
    for (int i = 0; i < 3; i++)
    {
        aux[i] = left[i][2];
        aux2[i] = down[0][i];
        aux3[i] = right[i][0];
        aux4[i] = up[2][i];
        down[0][i] = aux[i];
        up[2][i] = aux3[i];
    }

    for (int i = 0; i < 3; i++)
    {
        left[i][2] = aux4[2-i];
        right[i][0] = aux2[2-i];
    }

    //Movimiento circular
    for (int i = 0; i < 3; i++)
    {
        aux[i] = front[0][i];
        aux2[i] = front[i][0];
        aux3[i] = front[2][i];
        aux4[i] = front[i][2];
    }

    for (int i = 0; i < 3; i++)
    {
        front[0][i] = aux4[i];
        front[i][0] = aux[2-i];
        front[2][i] = aux2[i];
        front[i][2] = aux3[2-i];
    }

    //Liberar memoria
    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);

}

void B()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento Lineal
    for (int i = 0; i < 3; i++)
    {
        aux[i] = left[i][0];
        aux2[i] = down[2][i];
        aux3[i] = right[i][2];
        aux4[i] = up[0][i];
        down[2][i] = aux[i];
        up[0][i] = aux3[i];
    }

    for (int i = 0; i < 3; i++)
    {
        left[i][0] = aux4[2-i];
        right[i][2] = aux2[2-i];
    }

    //Movimiento circular
    for (int i = 0; i < 3; i++)
    {
        aux[i] = back[0][i];
        aux2[i] = back[i][2];
        aux3[i] = back[2][i];
        aux4[i] = back[i][0];
    }

    for (int i = 0; i < 3; i++)
    {
        back[0][i] = aux4[2-i];
        back[i][2] = aux[i];
        back[2][i] = aux2[2-i];
        back[i][0] = aux3[i];
    }

    //Liberar memoria
    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);
}

void BP()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento lineal
    for (int i = 0; i < 3; i++)
    {
        aux[i] = right[i][2];
        aux2[i] = down[2][i];
        aux3[i] = left[i][0];
        aux4[i] = up[0][i];
        right[i][2] = aux4[i];
        left[i][0] = aux2[i];
    }

    for (int i = 0; i < 3; i++)
    {
        down[2][i] = aux[2-i];
        up[0][i] = aux3[2-i];
    }

    //Movimiento circular
    for (int i = 0; i < 3; i++)
    {
        aux[i] = back[0][i];
        aux2[i] = back[i][0];
        aux3[i] = back[2][i];
        aux4[i] = back[i][2];
    }

    for (int i = 0; i < 3; i++)
    {
        back[0][i] = aux4[i];
        back[i][0] = aux[2-i];
        back[2][i] = aux2[i];
        back[i][2] = aux3[2-i];
    }

    //Liberar memoria
    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);
}

void U()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento lineal
    for (int i = 0; i < 3; i++)
    {
        aux[i] = front[0][i];
        aux2[i] = left[0][i];
        aux3[i] = back[0][i];
        aux4[i] = right[0][i];
        front[0][i] = aux4[i];
        left[0][i] = aux[i];
        back[0][i] = aux2[i];
        right[0][i] = aux3[i];
    }

    //Movimiento circular
    for (int i = 0; i < 3; i++)
    {
        aux[i] = up[0][i];
        aux2[i] = up[i][2];
        aux3[i] = up[2][i];
        aux4[i] = up[i][0];
    }

    for (int i = 0; i < 3; i++)
    {
        up[0][i] = aux4[2-i];
        up[i][2] = aux[i];
        up[2][i] = aux2[2-i];
        up[i][0] = aux3[i];
    }
    //Liberar memoria
    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);
}

void UP()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento lineal
    for (int i = 0; i < 3; i++)
    {
        aux[i] = front[0][i];
        aux2[i] = right[0][i];
        aux3[i] = back[0][i];
        aux4[i] = left[0][i];
        front[0][i] = aux4[i];
        right[0][i] = aux[i];
        back[0][i] = aux2[i];
        left[0][i] = aux3[i];
    }

    //Movimiento circular
    for (int i = 0; i < 3; i++)
    {
        aux[i] = up[0][i];
        aux2[i] = up[i][0];
        aux3[i] = up[2][i];
        aux4[i] = up[i][2];
    }

    for (int i = 0; i < 3; i++)
    {
        up[0][i] = aux4[i];
        up[i][0] = aux[2-i];
        up[2][i] = aux2[i];
        up[i][2] = aux3[2-i];
    }

    //Liberar memoria
    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);
}

void D()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento lineal
    for (int i = 0; i < 3; i++)
    {
        aux[i] = front[2][i];
        aux2[i] = right[2][i];
        aux3[i] = back[2][i];
        aux4[i] = left[2][i];
        front[2][i] = aux4[i];
        right[2][i] = aux[i];
        back[2][i] = aux2[i];
        left[2][i] = aux3[i];
    }

    //Movimiento circular
    for (int i = 0; i < 3; i++)
    {
        aux[i] = down[0][i];
        aux2[i] = down[i][2];
        aux3[i] = down[2][i];
        aux4[i] = down[i][0];
    }

    for (int i = 0; i < 3; i++)
    {
        down[0][i] = aux4[2-i];
        down[i][2] = aux[i];
        down[2][i] = aux2[2-i];
        down[i][0] = aux3[i];
    }

    //Liberar memoria
    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);
}

void DP()
{
    //Array Auxiliar
    int *aux = malloc(3*sizeof(int));
    int *aux2 = malloc(3*sizeof(int));
    int *aux3 = malloc(3*sizeof(int));
    int *aux4 = malloc(3*sizeof(int));

    //Movimiento lineal
    for (int i = 0; i < 3; i++)
    {
        aux[i] = front[2][i];
        aux2[i] = left[2][i];
        aux3[i] = back[2][i];
        aux4[i] = right[2][i];
        front[2][i] = aux4[i];
        left[2][i] = aux[i];
        back[2][i] = aux2[i];
        right[2][i] = aux3[i];
    }

    //Movimiento circular
    for (int i = 0; i < 3; i++)
    {
        aux[i] = down[0][i];
        aux2[i] = down[i][0];
        aux3[i] = down[2][i];
        aux4[i] = down[i][2];
    }

    for (int i = 0; i < 3; i++)
    {
        down[0][i] = aux4[i];
        down[i][0] = aux[2-i];
        down[2][i] = aux2[i];
        down[i][2] = aux3[2-i];
    }

    //Liberar memoria
    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////                                              Algoritmos de PLL                                 ///////////////////////////////////////////////////////////////////////////////////////////////////

void PLLU1() //Aristas antihorario
{
    R(); R(); UP(); RP();
    UP(); R(); U(); R();
    U(); R(); UP(); R();
}
void PLLU2() //Aristas horario
{
    RP(); U(); RP(); UP(); RP(); UP();
    RP(); U(); R(); U(); R(); R();
}
void PLLZ() //Aristas adyacentes
{
    R(); L(); R(); L();
    DP();
    R(); L(); R(); L();
    UP();
    R(); LP();
    B(); B();
    R(); L(); R(); L();
    F(); F();
    R(); LP();
    U(); U();
}
void PLLH() //Aristas opuestas (en cruz)
{
    R(); L(); R(); L();
    D();
    R(); L(); R(); L();
    U(); U();
    R(); L(); R(); L();
    D();
    R(); L(); R(); L();

}
void PLLSKIP() //Reduce PLL a 4 algoritmmos
{
    LP(); B(); LP(); F(); F();
    L(); BP(); LP(); F(); F();
    L(); L();
}
int PLLCHECK()
{
    int val = 0;
    //Revisa la primera fila por igualdad, regresa el numero de cara
    /*
        1 Front
        2 Right
        3 Back
        4 Left
    */
    if (front[0][0] == front[0][1] && front[0][1] == front[0][2])
        val = 2;

    if (right[0][0] == right[0][1] && right[0][1] == right[0][2])
        val = 3;

    if (back[0][0] == back[0][1] && back[0][1] == back[0][2])
        val = 4;

    if (left[0][0] == left[0][1] && left[0][1] == left[0][2])
        val = 5;

    if ((front[0][0] == front[0][1] && front[0][1] == front[0][2]) &&
       (right[0][0] == right[0][1] && right[0][1] == right[0][2]) &&
       (back[0][0] == back[0][1] && back[0][1] == back[0][2]) &&
       (left[0][0] == left[0][1] && left[0][1] == left[0][2]))
        val = 6;

    return val;
}

int isOpposite(int a, int b)
{
    //Se empieza asumiendo que los colores A y B NO son opuestos
    int val = 0;

    //Si Blanco y Amarillo
    if ((a == 1 && b == 6) || (a == 6 && b == 1))
        val = 1;

    if ((a == 2 && b == 4) || (a == 4 && b == 2))
        val = 1;

    if ((a == 3 && b == 5) || (a == 5 && b == 3))
        val = 1;

    return val;
}

int isCubeDone()
{
    //Regresa 1 si el cubo est� hecho
    //Se empieza suponiendo que el cubo S� est� hecho
    int val = 1;

    //Guarda los colores de los centros para comparar con el resto del cubo
    int *color1 = malloc(sizeof(int));
    int *color2 = malloc(sizeof(int));
    int *color3 = malloc(sizeof(int));
    int *color4 = malloc(sizeof(int));
    int *color5 = malloc(sizeof(int));
    int *color6 = malloc(sizeof(int));

    *color1 = down[1][1];
    *color2 = front[1][1];
    *color3 = right[1][1];
    *color4 = back[1][1];
    *color5 = left[1][1];
    *color6 = up[1][1];

    for (int i = 0; i < 3; i ++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (down[i][j] == *color1 && front[i][j] == *color2 && right[i][j] == *color3 && back[i][j] == *color4 && left[i][j] == *color5 && up[i][j]== *color6)
            {

            }
            else
                val = 0;
        }
    }
    //Liberar memoria
    free(color1);
    free(color2);
    free(color3);
    free(color4);
    free(color5);
    free(color6);

    return val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////                                              Algoritmos de OLL                                 ///////////////////////////////////////////////////////////////////////////////////////////////////
void OLLDOT()
{
    F(); R(); U(); RP(); UP(); FP();
}
void OLLLINE()
{
    F(); R(); U(); RP(); UP(); FP();
}
void OLLL()
{
    F(); U(); R(); UP(); RP(); FP();
}
void OLLA1() //Cruz f�cil
{
    R(); U(); U(); RP(); UP();
    R(); U(); RP(); UP();
    R(); UP(); RP();
}
void OLLA2() //Cruz (nave)
{
    R(); U(); U(); R(); R(); UP(); R(); R();
    UP(); R(); R(); U(); U(); R();
}
void OLLA3() //Pez 1
{
    LP(); U(); R(); UP(); L(); U(); RP();
}
void OLLA4() //Pez 2
{
    R(); UP(); LP(); U(); RP(); UP(); L();
}
void OLLA5() //Nave
{
    R(); R(); DP(); R(); U(); U(); RP(); D();
    R(); U(); U(); R();
}
void OLLA6() //Nave 2
{
    L(); F(); RP(); FP();
    LP(); F(); R(); FP();
}
void OLLA7() //Mo�o
{
    RP(); FP(); LP(); F(); R(); FP(); L(); F();
}
int isCrossDone()
{
    //Se presupone que NO hay cruz
    int val = 0;

    //Guarda el color del centro de la cara superior
    int *color = malloc(sizeof(int));
    *color = up[1][1];

    if (up[0][1] == *color && up[1][0] == *color && up[1][2] == *color && up[2][1] == *color )
        val = 1;

    free(color);
    return val;
}
int isL() //Regresa 1 si hay una L (antes de la cruz)
{
    int val = 0;

    //Guarda el color del centro de la cara superior
    int *color = malloc(sizeof(int));
    *color = up[1][1];

    if ((up[0][1] == *color && up[1][0] == *color && up[1][2] != *color && up[2][1] != *color) ||
       (up[0][1] == *color && up[1][2] == *color && up[1][0] != *color && up[2][1] != *color) ||
       (up[1][2] == *color && up[2][1] == *color && up[0][1] != *color && up[1][0] != *color) ||
       (up[1][0] == *color && up[2][1] == *color && up[0][1] != *color && up[1][2] != *color))
       val = 1;

    free(color);
    return val;
}
int isDot() //Regresa 1 si s�lo hay un punto (antes de la cruz)
{
    int val = 0;

    //Guarda el color del centro de la cara superior
    int *color = malloc(sizeof(int));
    *color = up[1][1];

    if (up[0][1] != *color && up[1][0] != *color && up[1][2] != *color && up[2][1] != *color)
        val = 1;

    free(color);
    return val;
}
int isLine() //Regresa 1 si hay una l�nea (antes de cruz)
{
    int val = 0;

    //Guarda el color del centro de la cara superior
    int *color = malloc(sizeof(int));
    *color = up[1][1];

    if ((up[0][1] == *color && up[2][1] == *color) || (up[1][0] == *color && up[1][2] == *color))
        val = 1;

    free(color);
    return val;
}
int isLastLayerDone()
{
    //Se empieza suponiendo que la cara est� hecha
    int val = 1;

    int *color = malloc(sizeof(int));
    *color = up[1][1];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (up[i][j] == *color)
            {

            }
            else
                val = 0;
        }
    }

    free(color);
    return val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////                                              Algoritmos de F2L                                 ///////////////////////////////////////////////////////////////////////////////////////////////////

int getCornerPosition(int corner) //Devuelve el n�mero en donde se encuentra la esquina actual
{
    //Valor a devolver
    int position = 0;

    //Variables para colores
    int *frontColor = malloc(sizeof(int));
    int *rightColor = malloc(sizeof(int));
    int *backColor = malloc(sizeof(int));
    int *leftColor = malloc(sizeof(int));
    int *upColor = malloc(sizeof(int));
    int *downColor = malloc(sizeof(int));

    //Colores actuales
    *frontColor = front[1][1];
    *rightColor = right[1][1];
    *backColor = back[1][1];
    *leftColor = left[1][1];
    *upColor = up[1][1];
    *downColor = down[1][1];

    switch (corner)
    {
        case 1:
            //Empezar inspecci�n del cubo para esquina EDFL
            if (down[0][0] == *downColor)
            {
                if (left[2][2] == *leftColor && front[2][0] == *frontColor)
                {
                    position = 1;
                    break;
                }
            }
            if (down[0][2] == *downColor)
            {
                if (front[2][2] == *leftColor && right[2][0] == *frontColor)
                {
                    position = 3;
                    break;
                }
            }
            if (down[2][0] == *downColor)
            {
                if (back[2][2] == *leftColor && left[2][0] == *frontColor)
                {
                    position = 7;
                    break;
                }
            }
            if (down[2][2] == *downColor)
            {
                if (right[2][2] == *leftColor && back[2][0] == *frontColor)
                {
                    position = 5;
                    break;
                }
            }


            if (front[0][0] == *downColor)
            {
                if (left[0][2] == *leftColor && up[2][0] == *frontColor)
                {
                    position = 13;
                    break;
                }
            }
            if (front[0][2] == *downColor)
            {
                if (up[2][2] == *leftColor && right[0][0] == *frontColor)
                {
                    position = 15;
                    break;
                }
            }
            if (front[2][0] == *downColor)
            {
                if (down[0][0] == *leftColor && left[2][2] == *frontColor)
                {
                    position = 1;
                    break;
                }
            }
            if (front[2][2] == *downColor)
            {
                if (right[2][0] == *leftColor && down[0][2] == *frontColor)
                {
                    position = 3;
                    break;
                }
            }

            if (right[0][0] == *downColor)
            {
                if (front[0][2] == *leftColor && up[2][2] == *frontColor)
                {
                    position = 15;
                    break;
                }
            }
            if (right[0][2] == *downColor)
            {
                if (up[0][2] == *leftColor && back[0][0] == *frontColor)
                {
                    position = 17;
                    break;
                }
            }
            if (right[2][0] == *downColor)
            {
                if (down[0][2] == *leftColor && front[2][2] == *frontColor)
                {
                    position = 3;
                    break;
                }
            }
            if (right[2][2] == *downColor)
            {
                if (back[2][0] == *leftColor && down[2][2] == *frontColor)
                {
                    position = 5;
                    break;
                }
            }

            if (back[0][0] == *downColor)
            {
                if (right[0][2] == *leftColor && up[0][2] == *frontColor)
                {
                    position = 17;
                    break;
                }
            }
            if (back[0][2] == *downColor)
            {
                if (up[0][0] == *leftColor && left[0][0] == *frontColor)
                {
                    position = 19;
                    break;
                }
            }
            if (back[2][0] == *downColor)
            {
                if (down[2][2] == *leftColor && right[2][2] == *frontColor)
                {
                    position = 5;
                    break;
                }
            }
            if (back[2][2] == *downColor)
            {
                if (left[2][0] == *leftColor && down[2][0] == *frontColor)
                {
                    position = 7;
                    break;
                }
            }

            if (left[0][0] == *downColor)
            {
                if (back[0][2] == *leftColor && up[0][0] == *frontColor)
                {
                    position = 19;
                    break;
                }
            }
            if (left[0][2] == *downColor)
            {
                if (up[2][0] == *leftColor && front[0][0] == *frontColor)
                {
                    position = 13;
                    break;
                }
            }
            if (left[2][0] == *downColor)
            {
                if (down[2][0] == *leftColor && back[2][2] == *frontColor)
                {
                    position = 7;
                    break;
                }
            }
            if (left[2][2] == *downColor)
            {
                if (front[2][0] == *leftColor && down[0][0] == *frontColor)
                {
                    position = 1;
                    break;
                }
            }

            if (up[0][0] == *downColor)
            {
                if (left[0][0] == *leftColor && back[0][2] == *frontColor)
                {
                    position = 19;
                    break;
                }
            }
            if (up[0][2] == *downColor)
            {
                if (back[0][0] == *leftColor && right[0][2] == *frontColor)
                {
                    position = 17;
                    break;
                }
            }
            if (up[2][0] == *downColor)
            {
                if (front[0][0] == *leftColor && left[0][2] == *frontColor)
                {
                    position = 13;
                    break;
                }
            }
            if (up[2][2] == *downColor)
            {
                if (right[0][0] == *leftColor && front[0][2] == *frontColor)
                {
                    position = 15;
                    break;
                }
            }

        case 2:
            //Empezar inspecci�n del cubo para esquina EDFR

            if (down[0][0] == *downColor)
            {
                if (left[2][2] == *frontColor && front[2][0] == *rightColor)
                {
                    position = 1;
                    break;
                }
            }
            if (down[0][2] == *downColor)
            {
                if (front[2][2] == *frontColor && right[2][0] == *rightColor)
                {
                    position = 3;
                    break;
                }
            }
            if (down[2][0] == *downColor)
            {
                //Los valores complementarios son left[2,0] y back[2,2]
                if (back[2][2] == *frontColor && left[2][0] == *rightColor)
                {
                    position = 7;
                    break;
                }
            }
            if (down[2][2] == *downColor)
            {
                //Los valores complementarios son right[2,2] y back[2,0]
                if (right[2][2] == *frontColor && back[2][0] == *rightColor)
                {
                    position = 5;
                    break;
                }
            }

            //Si la esquina est� en la cara frontal
            if (front[0][0] == *downColor)
            {
                //Los valores complementarios son left[0][2] y up[2][0]
                if (left[0][2] == *frontColor && up[2][0] == *rightColor)
                {
                    position = 13;
                    break;
                }
            }
            if (front[0][2] == *downColor)
            {
                //Los valores complementarios son right[0][0] y up[2][2]
                if (up[2][2] == *frontColor && right[0][0] == *rightColor)
                {
                    position = 15;
                    break;
                }
            }
            if (front[2][0] == *downColor)
            {
                //Los valores complementarios son down[0][0] y left[2][2]
                if (down[0][0] == *frontColor && left[2][2] == *rightColor)
                {
                    position = 1;
                    break;
                }
            }
            if (front[2][2] == *downColor)
            {
                //Los valores complementarios son down[0][2] y right[2][0]
                if (right[2][0] == *frontColor && down[0][2] == *rightColor)
                {
                    position = 3;
                    break;
                }
            }

            //Si la esquina est� en la cara derecha
            if (right[0][0] == *downColor)
            {
                //Los valores complementarios son front[0][2] y up[2][2]
                if (front[0][2] == *frontColor && up[2][2] == *rightColor)
                {
                    position = 15;
                    break;
                }
            }
            if (right[0][2] == *downColor)
            {
                //Los valores complementarios son up[0][2] y back[0][0]
                if (up[0][2] == *frontColor && back[0][0] == *rightColor)
                {
                    position = 17;
                    break;
                }
            }
            if (right[2][0] == *downColor)
            {
                //Los valores complementarios son down[0][2] y front[2][2]
                if (down[0][2] == *frontColor && front[2][2] == *rightColor)
                {
                    position = 3;
                    break;
                }
            }
            if (right[2][2] == *downColor)
            {
                //Los valores complementarios son down[2][2] y back[2][0]
                if (back[2][0] == *frontColor && down[2][2] == *rightColor)
                {
                    position = 5;
                    break;
                }
            }

            //Si la esquina est� en la cara trasera
            if (back[0][0] == *downColor)
            {
                //Los valores complementarios son right[0][2] y up[0][2]
                if (right[0][2] == *frontColor && up[0][2] == *rightColor)
                {
                    position = 17;
                    break;
                }
            }
            if (back[0][2] == *downColor)
            {
                //Los valores complementarios son up[0][0] y left[0][0]
                if (up[0][0] == *frontColor && left[0][0] == *rightColor)
                {
                    position = 19;
                    break;
                }
            }
            if (back[2][0] == *downColor)
            {
                //Los valores complementarios son down[2][2] y right[2][2]
                if (down[2][2] == *frontColor && right[2][2] == *rightColor)
                {
                    position = 5;
                    break;
                }
            }
            if (back[2][2] == *downColor)
            {
                //Los valores complementarios son left[2][0] y down[2][0]
                if (left[2][0] == *frontColor && down[2][0] == *rightColor)
                {
                    position = 7;
                    break;
                }
            }

            //Si la esquina est� en la cara izquierda
            if (left[0][0] == *downColor)
            {
                //Los valores complementarios son back[0][2] y up[0][0]
                if (back[0][2] == *frontColor && up[0][0] == *rightColor)
                {
                    position = 19;
                    break;
                }
            }
            if (left[0][2] == *downColor)
            {
                //Los valores complementarios son up[2][0] y front[0][0]
                if (up[2][0] == *frontColor && front[0][0] == *rightColor)
                {
                    position = 13;
                    break;
                }
            }
            if (left[2][0] == *downColor)
            {
                //Los valores complementarios down[2][0] y back[2][2]
                if (down[2][0] == *frontColor && back[2][2] == *rightColor)
                {
                    position = 7;
                    break;
                }
            }
            if (left[2][2] == *downColor)
            {
                //Los valores complementarios front[2][0] y down[0][0]
                if (front[2][0] == *frontColor && down[0][0] == *rightColor)
                {
                    position = 1;
                    break;
                }
            }

            //Si la esquina est� en la cara de arriba
            if (up[0][0] == *downColor)
            {
                //Los valores complementarios left[0][0] y back[0][2]
                if (left[0][0] == *frontColor && back[0][2] == *rightColor)
                {
                    position = 19;
                    break;
                }
            }
            if (up[0][2] == *downColor)
            {
                //Los valores complementarios back[0][0] y right[0][2]
                if (back[0][0] == *frontColor && right[0][2] == *rightColor)
                {
                    position = 17;
                    break;
                }
            }
            if (up[2][0] == *downColor)
            {
                //Los valores complementarios front[0][0] y left[0][2]
                if (front[0][0] == *frontColor && left[0][2] == *rightColor)
                {
                    position = 13;
                    break;
                }
            }
            if (up[2][2] == *downColor)
            {
                //Los valores complementarios son right[0][0] y front[0][2]
                if (right[0][0] == *frontColor && front[0][2] == *rightColor)
                {
                    position = 15;
                    break;
                }
            }

        case 3:
            //Empezar inspecci�n del cubo para esquina EDRB

            //Si la esquina est� en la cara de abajo
            if (down[0][0] == *downColor)
            {
                //Los valores complementarios son front[2,0] y left[2,2]
                if (left[2][2] == *rightColor && front[2][0] == *backColor)
                {
                    position = 1;
                    break;
                }
            }
            if (down[0][2] == *downColor)
            {
                //Los valores complementarios son front[2,2] y right[2,0]
                if (front[2][2] == *rightColor && right[2][0] == *backColor)
                {
                    position = 3;
                    break;
                }
            }
            if (down[2][0] == *downColor)
            {
                //Los valores complementarios son left[2,0] y back[2,2]
                if (back[2][2] == *rightColor && left[2][0] == *backColor)
                {
                    position = 7;
                    break;
                }
            }
            if (down[2][2] == *downColor)
            {
                //Los valores complementarios son right[2,2] y back[2,0]
                if (right[2][2] == *rightColor && back[2][0] == *backColor)
                {
                    position = 5;
                    break;
                }
            }

            //Si la esquina est� en la cara frontal
            if (front[0][0] == *downColor)
            {
                //Los valores complementarios son left[0][2] y up[2][0]
                if (left[0][2] == *rightColor && up[2][0] == *backColor)
                {
                    position = 13;
                    break;
                }
            }
            if (front[0][2] == *downColor)
            {
                //Los valores complementarios son right[0][0] y up[2][2]
                if (up[2][2] == *rightColor && right[0][0] == *backColor)
                {
                    position = 15;
                    break;
                }
            }
            if (front[2][0] == *downColor)
            {
                //Los valores complementarios son down[0][0] y left[2][2]
                if (down[0][0] == *rightColor && left[2][2] == *backColor)
                {
                    position = 1;
                    break;
                }
            }
            if (front[2][2] == *downColor)
            {
                //Los valores complementarios son down[0][2] y right[2][0]
                if (right[2][0] == *rightColor && down[0][2] == *backColor)
                {
                    position = 3;
                    break;
                }
            }

            //Si la esquina est� en la cara derecha
            if (right[0][0] == *downColor)
            {
                //Los valores complementarios son front[0][2] y up[2][2]
                if (front[0][2] == *rightColor && up[2][2] == *backColor)
                {
                    position = 15;
                    break;
                }
            }
            if (right[0][2] == *downColor)
            {
                //Los valores complementarios son up[0][2] y back[0][0]
                if (up[0][2] == *rightColor && back[0][0] == *backColor)
                {
                    position = 17;
                    break;
                }
            }
            if (right[2][0] == *downColor)
            {
                //Los valores complementarios son down[0][2] y front[2][2]
                if (down[0][2] == *rightColor && front[2][2] == *backColor)
                {
                    position = 3;
                    break;
                }
            }
            if (right[2][2] == *downColor)
            {
                //Los valores complementarios son down[2][2] y back[2][0]
                if (back[2][0] == *rightColor && down[2][2] == *backColor)
                {
                    position = 5;
                    break;
                }
            }

            //Si la esquina est� en la cara trasera
            if (back[0][0] == *downColor)
            {
                //Los valores complementarios son right[0][2] y up[0][2]
                if (right[0][2] == *rightColor && up[0][2] == *backColor)
                {
                    position = 17;
                    break;
                }
            }
            if (back[0][2] == *downColor)
            {
                //Los valores complementarios son up[0][0] y left[0][0]
                if (up[0][0] == *rightColor && left[0][0] == *backColor)
                {
                    position = 19;
                    break;
                }
            }
            if (back[2][0] == *downColor)
            {
                //Los valores complementarios son down[2][2] y right[2][2]
                if (down[2][2] == *rightColor && right[2][2] == *backColor)
                {
                    position = 5;
                    break;
                }
            }
            if (back[2][2] == *downColor)
            {
                //Los valores complementarios son left[2][0] y down[2][0]
                if (left[2][0] == *rightColor && down[2][0] == *backColor)
                {
                    position = 7;
                    break;
                }
            }

            //Si la esquina est� en la cara izquierda
            if (left[0][0] == *downColor)
            {
                //Los valores complementarios son back[0][2] y up[0][0]
                if (back[0][2] == *rightColor && up[0][0] == *backColor)
                {
                    position = 19;
                    break;
                }
            }
            if (left[0][2] == *downColor)
            {
                //Los valores complementarios son up[2][0] y front[0][0]
                if (up[2][0] == *rightColor && front[0][0] == *backColor)
                {
                    position = 13;
                    break;
                }
            }
            if (left[2][0] == *downColor)
            {
                //Los valores complementarios down[2][0] y back[2][2]
                if (down[2][0] == *rightColor && back[2][2] == *backColor)
                {
                    position = 7;
                    break;
                }
            }
            if (left[2][2] == *downColor)
            {
                //Los valores complementarios front[2][0] y down[0][0]
                if (front[2][0] == *rightColor && down[0][0] == *backColor)
                {
                    position = 1;
                    break;
                }
            }

            //Si la esquina est� en la cara de arriba
            if (up[0][0] == *downColor)
            {
                //Los valores complementarios left[0][0] y back[0][2]
                if (left[0][0] == *rightColor && back[0][2] == *backColor)
                {
                    position = 19;
                    break;
                }
            }
            if (up[0][2] == *downColor)
            {
                //Los valores complementarios back[0][0] y right[0][2]
                if (back[0][0] == *rightColor && right[0][2] == *backColor)
                {
                    position = 17;
                    break;
                }
            }
            if (up[2][0] == *downColor)
            {
                //Los valores complementarios front[0][0] y left[0][2]
                if (front[0][0] == *rightColor && left[0][2] == *backColor)
                {
                    position = 13;
                    break;
                }
            }
            if (up[2][2] == *downColor)
            {
                //Los valores complementarios son right[0][0] y front[0][2]
                if (right[0][0] == *rightColor && front[0][2] == *backColor)
                {
                    position = 15;
                    break;
                }
            }

        case 4:
            //Empezar inspecci�n del cubo para esquina EDBL

            //Si la esquina est� en la cara de abajo
            if (down[0][0] == *downColor)
            {
                //Los valores complementarios son front[2,0] y left[2,2]
                if (left[2][2] == *backColor && front[2][0] == *leftColor)
                {
                    position = 1;
                    break;
                }
            }
            if (down[0][2] == *downColor)
            {
                //Los valores complementarios son front[2,2] y right[2,0]
                if (front[2][2] == *backColor && right[2][0] == *leftColor)
                {
                    position = 3;
                    break;
                }
            }
            if (down[2][0] == *downColor)
            {
                //Los valores complementarios son left[2,0] y back[2,2]
                if (back[2][2] == *backColor && left[2][0] == *leftColor)
                {
                    position = 7;
                    break;
                }
            }
            if (down[2][2] == *downColor)
            {
                //Los valores complementarios son right[2,2] y back[2,0]
                if (right[2][2] == *backColor && back[2][0] == *leftColor)
                {
                    position = 5;
                    break;
                }
            }

            //Si la esquina est� en la cara frontal
            if (front[0][0] == *downColor)
            {
                //Los valores complementarios son left[0][2] y up[2][0]
                if (left[0][2] == *backColor && up[2][0] == *leftColor)
                {
                    position = 13;
                    break;
                }
            }
            if (front[0][2] == *downColor)
            {
                //Los valores complementarios son right[0][0] y up[2][2]
                if (up[2][2] == *backColor && right[0][0] == *leftColor)
                {
                    position = 15;
                    break;
                }
            }
            if (front[2][0] == *downColor)
            {
                //Los valores complementarios son down[0][0] y left[2][2]
                if (down[0][0] == *backColor && left[2][2] == *leftColor)
                {
                    position = 1;
                    break;
                }
            }
            if (front[2][2] == *downColor)
            {
                //Los valores complementarios son down[0][2] y right[2][0]
                if (right[2][0] == *backColor && down[0][2] == *leftColor)
                {
                    position = 3;
                    break;
                }
            }

            //Si la esquina est� en la cara derecha
            if (right[0][0] == *downColor)
            {
                //Los valores complementarios son front[0][2] y up[2][2]
                if (front[0][2] == *backColor && up[2][2] == *leftColor)
                {
                    position = 15;
                    break;
                }
            }
            if (right[0][2] == *downColor)
            {
                //Los valores complementarios son up[0][2] y back[0][0]
                if (up[0][2] == *backColor && back[0][0] == *leftColor)
                {
                    position = 17;
                    break;
                }
            }
            if (right[2][0] == *downColor)
            {
                //Los valores complementarios son down[0][2] y front[2][2]
                if (down[0][2] == *backColor && front[2][2] == *leftColor)
                {
                    position = 3;
                    break;
                }
            }
            if (right[2][2] == *downColor)
            {
                //Los valores complementarios son down[2][2] y back[2][0]
                if (back[2][0] == *backColor && down[2][2] == *leftColor)
                {
                    position = 5;
                    break;
                }
            }

            if (back[0][0] == *downColor)
            {
                if (right[0][2] == *backColor && up[0][2] == *leftColor)
                {
                    position = 17;
                    break;
                }
            }
            if (back[0][2] == *downColor)
            {
                if (up[0][0] == *backColor && left[0][0] == *leftColor)
                {
                    position = 19;
                    break;
                }
            }
            if (back[2][0] == *downColor)
            {
                if (down[2][2] == *backColor && right[2][2] == *leftColor)
                {
                    position = 5;
                    break;
                }
            }
            if (back[2][2] == *downColor)
            {
                if (left[2][0] == *backColor && down[2][0] == *leftColor)
                {
                    position = 7;
                    break;
                }
            }

            //Si la esquina est� en la cara izquierda
            if (left[0][0] == *downColor)
            {
                if (back[0][2] == *backColor && up[0][0] == *leftColor)
                {
                    position = 19;
                    break;
                }
            }
            if (left[0][2] == *downColor)
            {
                if (up[2][0] == *backColor && front[0][0] == *leftColor)
                {
                    position = 13;
                    break;
                }
            }
            if (left[2][0] == *downColor)
            {
                if (down[2][0] == *backColor && back[2][2] == *leftColor)
                {
                    position = 7;
                    break;
                }
            }
            if (left[2][2] == *downColor)
            {
                if (front[2][0] == *backColor && down[0][0] == *leftColor)
                {
                    position = 1;
                    break;
                }
            }

            if (up[0][0] == *downColor)
            {
                if (left[0][0] == *backColor && back[0][2] == *leftColor)
                {
                    position = 19;
                    break;
                }
            }
            if (up[0][2] == *downColor)
            {
                if (back[0][0] == *backColor && right[0][2] == *leftColor)
                {
                    position = 17;
                    break;
                }
            }
            if (up[2][0] == *downColor)
            {
                if (front[0][0] == *backColor && left[0][2] == *leftColor)
                {
                    position = 13;
                    break;
                }
            }
            if (up[2][2] == *downColor)
            {
                if (right[0][0] == *backColor && front[0][2] == *leftColor)
                {
                    position = 15;
                    break;
                }
            }
    }

    //Liberar memoria
    free(frontColor);
    free(rightColor);
    free(backColor);
    free(leftColor);
    free(upColor);
    free(downColor);

    //Regresar posici�n
    return position;
}

int getEdgePosition(int arista)
{
    int position = 0;

    //Variables para colores
    int *frontColor = malloc(sizeof(int));
    int *rightColor = malloc(sizeof(int));
    int *backColor = malloc(sizeof(int));
    int *leftColor = malloc(sizeof(int));
    int *upColor = malloc(sizeof(int));
    int *downColor = malloc(sizeof(int));

    //Colores actuales
    *frontColor = front[1][1];
    *rightColor = right[1][1];
    *backColor = back[1][1];
    *leftColor = left[1][1];
    *upColor = up[1][1];
    *downColor = down[1][1];

    switch (arista)
    {
    case 1:
        //Arista 1 es AFL
        //Revisar si el arista se encuentra en la cara frontal
        if (front[0][1] == *frontColor && up[2][1] == *leftColor)
        {
            position = 14;
            break;
        }
        if (front[1][0] == *frontColor && left[1][2] == *leftColor)
        {
            position = 9;
            break;
        }
        if (front[1][2] == *frontColor && right[1][0] == *leftColor)
        {
            position = 10;
            break;
        }

        //Revisar si el arista se encuentra en la cara derecha
        if (right[0][1] == *frontColor && up[1][2] == *leftColor)
        {
            position = 16;
            break;
        }
        if (right[1][0] == *frontColor && front[1][2] == *leftColor)
        {
            position = 10;
            break;
        }
        if (right[1][2] == *frontColor && back[1][0] == *leftColor)
        {
            position = 11;
            break;
        }
        //Revisar si el arista se encuentra en la cara trasera
        if (back[0][1] == *frontColor && up[0][1] == *leftColor)
        {
            position = 18;
            break;
        }
        if (back[1][0] == *frontColor && right[1][2] == *leftColor)
        {
            position = 11;
            break;
        }
        if (back[1][2] == *frontColor && left[1][0] == *leftColor)
        {
            position = 12;
            break;
        }
        //Revisar si el arista se encuentra en la cara izquierda
        if (left[0][1] == *frontColor && up[1][0] == *leftColor)
        {
            position = 20;
            break;
        }
        if (left[1][0] == *frontColor && back[1][2] == *leftColor)
        {
            position = 12;
            break;
        }
        if (left[1][2] == *frontColor && left[1][0] == *leftColor)
        {
            position = 9;
            break;
        }
        //Revisar si se encuentra en la cara superior
        if (up[0][1] == *frontColor && back[0][1] == *leftColor)
        {
            position = 18;
            break;
        }
        if (up[1][0] == *frontColor && left[0][1] == *leftColor)
        {
            position = 20;
            break;
        }
        if (up[1][2] == *frontColor && right[0][1] == *leftColor)
        {
            position = 16;
            break;
        }
        if (up[1][2] == *frontColor && front[0][1] == *leftColor)
        {
            position = 14;
            break;
        }
    case 2:
        //Arista 2 es AFR
        //Revisar si el arista se encuentra en la cara frontal
        if (front[0][1] == *frontColor && up[2][1] == *rightColor)
        {
            position = 14;
            break;
        }
        if (front[1][0] == *frontColor && left[1][2] == *rightColor)
        {
            position = 9;
            break;
        }
        if (front[1][2] == *frontColor && right[1][0] == *rightColor)
        {
            position = 10;
            break;
        }

        //Revisar si el arista se encuentra en la cara derecha
        if (right[0][1] == *frontColor && up[1][2] == *rightColor)
        {
            position = 16;
            break;
        }
        if (right[1][0] == *frontColor && front[1][2] == *rightColor)
        {
            position = 10;
            break;
        }
        if (right[1][2] == *frontColor && back[1][0] == *rightColor)
        {
            position = 11;
            break;
        }
        //Revisar si el arista se encuentra en la cara trasera
        if (back[0][1] == *frontColor && up[0][1] == *rightColor)
        {
            position = 18;
            break;
        }
        if (back[1][0] == *frontColor && right[1][2] == *rightColor)
        {
            position = 11;
            break;
        }
        if (back[1][2] == *frontColor && left[1][0] == *rightColor)
        {
            position = 12;
            break;
        }
        //Revisar si el arista se encuentra en la cara izquierda
        if (left[0][1] == *frontColor && up[1][0] == *rightColor)
        {
            position = 20;
            break;
        }
        if (left[1][0] == *frontColor && back[1][2] == *rightColor)
        {
            position = 12;
            break;
        }
        if (left[1][2] == *frontColor && left[1][0] == *rightColor)
        {
            position = 9;
            break;
        }
        //Revisar si se encuentra en la cara superior
        if (up[0][1] == *frontColor && back[0][1] == *rightColor)
        {
            position = 18;
            break;
        }
        if (up[1][0] == *frontColor && left[0][1] == *rightColor)
        {
            position = 20;
            break;
        }
        if (up[1][2] == *frontColor && right[0][1] == *rightColor)
        {
            position = 16;
            break;
        }
        if (up[1][2] == *frontColor && front[0][1] == *rightColor)
        {
            position = 14;
            break;
        }

    case 3:
        //Arista 3 es ARB
        //Revisar si el arista se encuentra en la cara frontal
        if (front[0][1] == *rightColor && up[2][1] == *backColor)
        {
            position = 14;
            break;
        }
        if (front[1][0] == *rightColor && left[1][2] == *backColor)
        {
            position = 9;
            break;
        }
        if (front[1][2] == *rightColor && right[1][0] == *backColor)
        {
            position = 10;
            break;
        }

        //Revisar si el arista se encuentra en la cara derecha
        if (right[0][1] == *rightColor && up[1][2] == *backColor)
        {
            position = 16;
            break;
        }
        if (right[1][0] == *rightColor && front[1][2] == *backColor)
        {
            position = 10;
            break;
        }
        if (right[1][2] == *rightColor && back[1][0] == *backColor)
        {
            position = 11;
            break;
        }
        //Revisar si el arista se encuentra en la cara trasera
        if (back[0][1] == *rightColor && up[0][1] == *backColor)
        {
            position = 18;
            break;
        }
        if (back[1][0] == *rightColor && right[1][2] == *backColor)
        {
            position = 11;
            break;
        }
        if (back[1][2] == *rightColor && left[1][0] == *backColor)
        {
            position = 12;
            break;
        }
        //Revisar si el arista se encuentra en la cara izquierda
        if (left[0][1] == *rightColor && up[1][0] == *backColor)
        {
            position = 20;
            break;
        }
        if (left[1][0] == *rightColor && back[1][2] == *backColor)
        {
            position = 12;
            break;
        }
        if (left[1][2] == *rightColor && left[1][0] == *backColor)
        {
            position = 9;
            break;
        }
        //Revisar si se encuentra en la cara superior
        if (up[0][1] == *rightColor && back[0][1] == *backColor)
        {
            position = 18;
            break;
        }
        if (up[1][0] == *rightColor && left[0][1] == *backColor)
        {
            position = 20;
            break;
        }
        if (up[1][2] == *rightColor && right[0][1] == *backColor)
        {
            position = 16;
            break;
        }
        if (up[1][2] == *rightColor && front[0][1] == *backColor)
        {
            position = 14;
            break;
        }

    case 4:
        //Arista 4 es ABL
        //Revisar si el arista se encuentra en la cara frontal
        if (front[0][1] == *backColor && up[2][1] == *leftColor)
        {
            position = 14;
            break;
        }
        if (front[1][0] == *backColor && left[1][2] == *leftColor)
        {
            position = 9;
            break;
        }
        if (front[1][2] == *backColor && right[1][0] == *leftColor)
        {
            position = 10;
            break;
        }

        //Revisar si el arista se encuentra en la cara derecha
        if (right[0][1] == *backColor && up[1][2] == *leftColor)
        {
            position = 16;
            break;
        }
        if (right[1][0] == *backColor && front[1][2] == *leftColor)
        {
            position = 10;
            break;
        }
        if (right[1][2] == *backColor && back[1][0] == *leftColor)
        {
            position = 11;
            break;
        }
        //Revisar si el arista se encuentra en la cara trasera
        if (back[0][1] == *backColor && up[0][1] == *leftColor)
        {
            position = 18;
            break;
        }
        if (back[1][0] == *backColor && right[1][2] == *leftColor)
        {
            position = 11;
            break;
        }
        if (back[1][2] == *backColor && left[1][0] == *leftColor)
        {
            position = 12;
            break;
        }
        //Revisar si el arista se encuentra en la cara izquierda
        if (left[0][1] == *backColor && up[1][0] == *leftColor)
        {
            position = 20;
            break;
        }
        if (left[1][0] == *backColor && back[1][2] == *leftColor)
        {
            position = 12;
            break;
        }
        if (left[1][2] == *backColor && left[1][0] == *leftColor)
        {
            position = 9;
            break;
        }
        //Revisar si se encuentra en la cara superior
        if (up[0][1] == *backColor && back[0][1] == *leftColor)
        {
            position = 18;
            break;
        }
        if (up[1][0] == *backColor && left[0][1] == *leftColor)
        {
            position = 20;
            break;
        }
        if (up[1][2] == *backColor && right[0][1] == *leftColor)
        {
            position = 16;
            break;
        }
        if (up[1][2] == *backColor && front[0][1] == *leftColor)
        {
            position = 14;
            break;
        }
        break;

    }
    free(frontColor);
    free(rightColor);
    free(backColor);
    free(leftColor);
    free(downColor);
    free(upColor);

    return position;
}

void setBlock1() //Orienta bloque 1 (esquina y arista)
{
    //Se repite la instrucción hasta que el bloque esté acomodado para aplicar algún algoritmo
    while ((getCornerPosition(1) != 19 && getCornerPosition(1) != 17 && getCornerPosition(1) != 15 && getCornerPosition(1) != 13 && getCornerPosition(1) != 1) ||
           (getEdgePosition(1) != 20 && getEdgePosition(1) != 18 && getEdgePosition(1) != 16 && getEdgePosition(1) != 14 && getEdgePosition(1) != 9))
    {
        //Si el bloque no está acomodado, acomodarlo
        //¿Qué es lo que no está acomodaado? ¿La esquina? ¿El arista? ¿Los dos?
        //Si la esquina esta mal
        if (getCornerPosition(1) != 19 && getCornerPosition(1) != 17 && getCornerPosition(1) != 15 && getCornerPosition(1) != 13 && getCornerPosition(1) != 1)
        {
            //Acomodar esquina
            //Si la esquina no está acomodada, sólo puede haber 3 ubicaciones posibles, que son
        }
        //Si el arista esta mal
        if (getEdgePosition(1) != 20 && getEdgePosition(1) != 18 && getEdgePosition(1) != 16 && getEdgePosition(1) != 14 && getEdgePosition(1) != 9)
        {
            //Acomodar arista
        }

    }
}

void setBlock2() //Orienta bloque 2 (esquina y arista)
{
    //Se repite la instrucción hasta que el bloque esté acomodado para aplicar algún algoritmo
    while ((getCornerPosition(2) != 19 && getCornerPosition(2) != 17 && getCornerPosition(2) != 15 && getCornerPosition(2) != 13 && getCornerPosition(2) != 3) ||
           (getEdgePosition(2) != 20 && getEdgePosition(2) != 18 && getEdgePosition(2) != 16 && getEdgePosition(2) != 14 && getEdgePosition(2) != 10))
    {
    }
}

void setBlock3() //Orienta bloque 3 (esquina y arista)
{
    //Se repite la instrucción hasta que el bloque esté acomodado para aplicar algún algoritmo
    while ((getCornerPosition(3) != 19 && getCornerPosition(3) != 17 && getCornerPosition(3) != 15 && getCornerPosition(3) != 13 && getCornerPosition(3) != 5) ||
           (getEdgePosition(3) != 20 && getEdgePosition(3) != 18 && getEdgePosition(3) != 16 && getEdgePosition(3) != 14 && getEdgePosition(3) != 11))
    {
    }
}

void setBlock4() //Orienta bloque 4 (esquina y arista)
{
    //Se repite la instrucción hasta que el bloque esté acomodado para aplicar algún algoritmo
    while ((getCornerPosition(4) != 19 && getCornerPosition(4) != 17 && getCornerPosition(4) != 15 && getCornerPosition(4) != 13 && getCornerPosition(4) != 7) ||
           (getEdgePosition(4) != 20 && getEdgePosition(4) != 18 && getEdgePosition(4) != 16 && getEdgePosition(4) != 14 && getEdgePosition(4) != 12))
    {
    }
}


//Basicos
//Esquina apuntando con el blanco a un lateral
//Esquina apuntando con el blanco arriba
//Esquina en capa inferior arista cara superior
//Arista anclada y esquina en cara superior
//Arista y esquina ancladas
