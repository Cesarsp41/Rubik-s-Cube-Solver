//Algoritmo a imitar: Fridrich Reducido
/*
    Nombre: Cesar Soltero Perez
    Fecha: 28/09/2022

    Update: 10/10/2022

    Cambios a hacer:
    -Funcion F no es compatible con Arduino, cambiar a FR cuando se exporta (cuando se termine)
    -PrintFace NO ser? exportada a Arduino, s?lo es una funcion auxiliar
    -Validad colores (9 colores m?ximo de cada uno)
    -Funci?n para cancelar escaner

    Objetivos:
    -Terminar SolveBlock4
    -Hacer Cruz
    -Algoritmo para revolver
    -Validaciones
*/

#include <stdio.h>
#include <stdlib.h>

//Arrglos
int down[3][3] = { {0,0,0,},{0,0,0,},{0,0,0} };
int front[3][3] = { {0,0,0,},{0,0,0,},{0,0,0} };
int right[3][3] = { {0,0,0,},{0,0,0,},{0,0,0} };
int left[3][3] = { {0,0,0,},{0,0,0,},{0,0,0} };
int back[3][3] = { {0,0,0,},{0,0,0,},{0,0,0} };
int up[3][3] = { {0,0,0,},{0,0,0,},{0,0,0} };

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
int PLLCHECK(); //Revisa la primera fila por igualdad, regresa 0 si no hay igualdad, regresa 1-6 seg?n el numero de cara
int isOpposite(int a, int b); //Determina si dos colores son opuestos, regresa 0 si FALSO, 1 si VERDADERO
int isCubeDone(); //Inspecciona el cubo, dice si est? terminado

//OLL
void OLLDOT(); //Algoritmo para convertir el punto en cruz
void OLLLINE(); //Algoritmo para convertir la linea en cruz
void OLLL(); //Algoritmo para convertir la L en cruz
void OLLA1(); //Cruz f?cil
void OLLA2(); //Cruz (nave)
void OLLA3(); //Pez 1
void OLLA4(); //Pez 2
void OLLA5(); //Nave
void OLLA6(); //Nave 2
void OLLA7(); //Mo?o
int isCrossDone(); //Inspecciona la cara superior y regresa 0 si NO hay cruz, regresa 1 si la hay
int isL(); //Regresa 1 si hay una L (antes de la cruz)
int isDot(); //Regresa 1 si s?lo hay un punto (antes de la cruz)
int isLine(); //Regresa 1 si hay una l?nea (antes de cruz)
int isLastLayerDone(); //Regresa 1 si la cara superior ya se ha resuelto

//F2L
int getCornerPosition(int corner); //Duelve la posici?n (1-20) de la esquina down[][] que se quiere acomodar
int getEdgePosition(int arista); //Duelve la posici?n (1-20) de la arista que se quiere encontrar
void setBlock1(); //Orienta bloque 1 (esquina y arista)
void setBlock2(); //Orienta bloque 2 (esquina y arista)
void setBlock3(); //Orienta bloque 3 (esquina y arista)
void setBlock4(); //Orienta bloque 4 (esquina y arista)
void solveBlock1();
void solveBlock2(); //Resuelven los bloques
void solveBlock3();
void solveBlock4();

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

    fflush(stdin);

    //Solving phase
    setBlock1();
    solveBlock1();
	setBlock2();
	solveBlock2();
	setBlock3();
	solveBlock3();
	setBlock4();

    //Printing
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////                                         Cross, F2L, OLL, PLL                                  ///////////////////////////////////////////////////////////////////////////////////////////////////////+
//PLL
void PLL()
{
    //El cubo entra con la cara superior ya resuelta pero con esquinas y aristas desorientados

    //Mientras el cubo NO est? resuelto
    while (isCubeDone() == 0)
    {
        //Buscar coincidencias de color en esquinas
        if (front[0][0] == front[0][2] && right[0][0] == right[0][2] && back[0][0] == back[0][2] && left[0][0] == left[0][2])
        {
            //Si TODAS las esquinas coinciden, U1, U2, Z, H

            //El IF verifica Si hay una l?nea completa, U1, U2.
            if (PLLCHECK() > 0)
            {
                //El switch determina en qu? cara se encuentra la l?nea de U1 o U2 y la orienta a la cara frontal
                switch (PLLCHECK())
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

                //En este punto s?lo debe decidirse si es U1 o U2
                if (isOpposite(front[0][1], right[0][1]) == 1)
                    PLLU1();

                else
                    PLLU2();

            }

            //Si no hay una l?nea completa, solo puede ser Z o H
            else
            {
                //H
                if (front[0][0] == back[0][1])
                {
                    PLLH();
                    //Girar U hasta que el cubo est? resuelto
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
            //Por lo menos hay UNA coincidencia. Determinar en qu? cara se encuentra la coincidencia
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

            else if (left[0][0] == left[0][2])
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
        //?Qu? figura tengo?
        //Orientar figura
        //Hacer cruz

        if (isLine() == 1)
        {
            //Girar U hasta orientar la l?nea en horizontal
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
        //Se parte del hecho de que la cruz ya est? hecha, por lo tanto, (0,1)(1,0)(1,2)(2,1) ya estan cubiertos
        //con el color central, solo se necesitan verificar esquinas restantes

        if (front[0][0] == color && front[0][2] == color && back[0][0] == color && back[0][2] == color)
        {
            OLLA1(); //Cruz f?cil
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
            //Si ning?n caso est? orientado, girar U
            U();
    }
    //Orientar cruz
    //Resovler cruz
}
//F2L
void F2L()
{
    //Mientras F2L No est? resuelto (while), hacer F2L
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
            scanf("%i", &num);
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
            printf("%i ", arr[i][j]);
        }
        printf("\n");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////                                              Movimientos de caras                                 //////////////////////////////////////////////////////////////////////////////////////////////
void R()
{
    //Array Auxiliar
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        back[i][0] = aux2[2 - i]; //Back (C) el valor de Up(B(Aux2))
        down[i][2] = aux[2 - i]; //Down(D) toma el valor de Back(C (aux))
    }

    //Movimiento circular de cara afectada
    for (int i = 0; i < 3; i++)
    {
        aux[i] = right[0][i];
        aux2[i] = right[i][2];
        aux3[i] = right[2][i]; ////Guarda la informaci?n de la cara afectada para girarla sobre su propio eje
        aux4[i] = right[i][0];
    }

    for (int i = 0; i < 3; i++)
    {
        right[i][2] = aux[i];
        right[2][i] = aux2[2 - i];
        right[i][0] = aux3[i];
        right[0][i] = aux4[2 - i];
    }

    free(aux);
    free(aux2);
    free(aux3);
    free(aux4);
}
void RP()
{
    //Array Auxiliar
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        back[i][0] = aux2[2 - i];
        up[i][2] = aux3[2 - i];
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
        right[i][0] = aux[2 - i];
        right[2][i] = aux2[i];
        right[i][2] = aux3[2 - i];
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
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        back[i][2] = aux2[2 - i];
        up[i][0] = aux3[2 - i];
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
        left[0][i] = aux4[2 - i];
        left[i][2] = aux[i];
        left[2][i] = aux2[2 - i];
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
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        back[i][2] = aux2[2 - i];
        down[i][0] = aux3[2 - i];
    }

    //Movimiento circuilar
    for (int i = 0; i < 3; i++)
    {
        aux[i] = left[0][i];
        aux2[i] = left[i][0];
        aux3[i] = left[2][i];
        aux4[i] = left[i][2];
    }

    for (int i = 0; i < 3; i++)
    {
        left[0][i] = aux4[i];
        left[i][0] = aux[2 - i];
        left[2][i] = aux2[i];
        left[i][2] = aux3[2 - i];
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
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        down[0][i] = aux[2 - i];
        up[2][i] = aux3[2 - i];
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
        front[0][i] = aux4[2 - i];
        front[i][2] = aux[i];
        front[2][i] = aux2[2 - i];
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
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        left[i][2] = aux4[2 - i];
        right[i][0] = aux2[2 - i];
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
        front[i][0] = aux[2 - i];
        front[2][i] = aux2[i];
        front[i][2] = aux3[2 - i];
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
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        left[i][0] = aux4[2 - i];
        right[i][2] = aux2[2 - i];
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
        back[0][i] = aux4[2 - i];
        back[i][2] = aux[i];
        back[2][i] = aux2[2 - i];
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
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        down[2][i] = aux[2 - i];
        up[0][i] = aux3[2 - i];
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
        back[i][0] = aux[2 - i];
        back[2][i] = aux2[i];
        back[i][2] = aux3[2 - i];
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
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        up[0][i] = aux4[2 - i];
        up[i][2] = aux[i];
        up[2][i] = aux2[2 - i];
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
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        up[i][0] = aux[2 - i];
        up[2][i] = aux2[i];
        up[i][2] = aux3[2 - i];
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
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        down[0][i] = aux4[2 - i];
        down[i][2] = aux[i];
        down[2][i] = aux2[2 - i];
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
    int* aux = malloc(3 * sizeof(int));
    int* aux2 = malloc(3 * sizeof(int));
    int* aux3 = malloc(3 * sizeof(int));
    int* aux4 = malloc(3 * sizeof(int));

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
        down[i][0] = aux[2 - i];
        down[2][i] = aux2[i];
        down[i][2] = aux3[2 - i];
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
    //Regresa 1 si el cubo est? hecho
    //Se empieza suponiendo que el cubo S? est? hecho
    int val = 1;

    //Guarda los colores de los centros para comparar con el resto del cubo
    int* color1 = malloc(sizeof(int));
    int* color2 = malloc(sizeof(int));
    int* color3 = malloc(sizeof(int));
    int* color4 = malloc(sizeof(int));
    int* color5 = malloc(sizeof(int));
    int* color6 = malloc(sizeof(int));

    *color1 = down[1][1];
    *color2 = front[1][1];
    *color3 = right[1][1];
    *color4 = back[1][1];
    *color5 = left[1][1];
    *color6 = up[1][1];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (down[i][j] == *color1 && front[i][j] == *color2 && right[i][j] == *color3 && back[i][j] == *color4 && left[i][j] == *color5 && up[i][j] == *color6)
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
void OLLA1() //Cruz facil
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
void OLLA7() //Mono
{
    RP(); FP(); LP(); F(); R(); FP(); L(); F();
}
int isCrossDone()
{
    //Se presupone que NO hay cruz
    int val = 0;

    //Guarda el color del centro de la cara superior
    int* color = malloc(sizeof(int));
    *color = up[1][1];

    if (up[0][1] == *color && up[1][0] == *color && up[1][2] == *color && up[2][1] == *color)
        val = 1;

    free(color);
    return val;
}
int isL() //Regresa 1 si hay una L (antes de la cruz)
{
    int val = 0;

    //Guarda el color del centro de la cara superior
    int* color = malloc(sizeof(int));
    *color = up[1][1];

    if ((up[0][1] == *color && up[1][0] == *color && up[1][2] != *color && up[2][1] != *color) ||
        (up[0][1] == *color && up[1][2] == *color && up[1][0] != *color && up[2][1] != *color) ||
        (up[1][2] == *color && up[2][1] == *color && up[0][1] != *color && up[1][0] != *color) ||
        (up[1][0] == *color && up[2][1] == *color && up[0][1] != *color && up[1][2] != *color))
        val = 1;

    free(color);
    return val;
}
int isDot() //Regresa 1 si s?lo hay un punto (antes de la cruz)
{
    int val = 0;

    //Guarda el color del centro de la cara superior
    int* color = malloc(sizeof(int));
    *color = up[1][1];

    if (up[0][1] != *color && up[1][0] != *color && up[1][2] != *color && up[2][1] != *color)
        val = 1;

    free(color);
    return val;
}
int isLine() //Regresa 1 si hay una l?nea (antes de cruz)
{
    int val = 0;

    //Guarda el color del centro de la cara superior
    int* color = malloc(sizeof(int));
    *color = up[1][1];

    if ((up[0][1] == *color && up[2][1] == *color) || (up[1][0] == *color && up[1][2] == *color))
        val = 1;

    free(color);
    return val;
}
int isLastLayerDone()
{
    //Se empieza suponiendo que la cara est? hecha
    int val = 1;

    int* color = malloc(sizeof(int));
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

int getCornerPosition(int corner) //Devuelve el n?mero en donde se encuentra la esquina actual
{
    //Valor a devolver
    int position = 0;

    //Variables para colores
    int* frontColor = malloc(sizeof(int));
    int* rightColor = malloc(sizeof(int));
    int* backColor = malloc(sizeof(int));
    int* leftColor = malloc(sizeof(int));
    int* upColor = malloc(sizeof(int));
    int* downColor = malloc(sizeof(int));

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
        //Empezar inspecci?n del cubo para esquina EDFL
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
        break;

    case 2:
        //Empezar inspecci?n del cubo para esquina EDFR

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

        //Si la esquina est? en la cara frontal
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

        //Si la esquina est? en la cara derecha
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

        //Si la esquina est? en la cara trasera
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

        //Si la esquina est? en la cara izquierda
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

        //Si la esquina est? en la cara de arriba
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
        break;

    case 3:
        //Empezar inspecci?n del cubo para esquina EDRB

        //Si la esquina est? en la cara de abajo
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

        //Si la esquina est? en la cara frontal
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

        //Si la esquina est? en la cara derecha
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

        //Si la esquina est? en la cara trasera
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

        //Si la esquina est? en la cara izquierda
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

        //Si la esquina est? en la cara de arriba
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
        break;

    case 4:
        //Empezar inspecci?n del cubo para esquina EDBL

        //Si la esquina est? en la cara de abajo
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

        //Si la esquina est? en la cara frontal
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

        //Si la esquina est? en la cara derecha
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

        //Si la esquina est? en la cara izquierda
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
        break;
    }

    //Liberar memoria
    free(frontColor);
    free(rightColor);
    free(backColor);
    free(leftColor);
    free(upColor);
    free(downColor);

    //Regresar posici?n
    return position;
}

int getEdgePosition(int arista)
{
    int position = 0;

    //Variables para colores
    int* frontColor = malloc(sizeof(int));
    int* rightColor = malloc(sizeof(int));
    int* backColor = malloc(sizeof(int));
    int* leftColor = malloc(sizeof(int));
    int* upColor = malloc(sizeof(int));
    int* downColor = malloc(sizeof(int));

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
        if (left[1][2] == *frontColor && front[1][0] == *leftColor)
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
        if (up[2][1] == *frontColor && front[0][1] == *leftColor)
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
        if (left[1][2] == *frontColor && front[1][0] == *rightColor)
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
        if (up[2][1] == *frontColor && front[0][1] == *rightColor)
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
        if (left[1][2] == *rightColor && front[1][0] == *backColor)
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
        if (up[2][1] == *rightColor && front[0][1] == *backColor)
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
        if (left[1][2] == *backColor && front[1][0] == *leftColor)
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
        if (up[2][1] == *backColor && front[0][1] == *leftColor)
        {
            position = 14;
            break;
        }

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
    //Se repite la instrucci?n hasta que el bloque est? acomodado para aplicar alg?n algoritmo
    while ((getCornerPosition(1) != 19 && getCornerPosition(1) != 17 && getCornerPosition(1) != 15 && getCornerPosition(1) != 13 && getCornerPosition(1) != 1) ||
        (getEdgePosition(1) != 20 && getEdgePosition(1) != 18 && getEdgePosition(1) != 16 && getEdgePosition(1) != 14 && getEdgePosition(1) != 9))
    {
        //Si el bloque no est? acomodado, acomodarlo
        //?Qu? es lo que no est? acomodaado? ?La esquina? ?El arista? ?Los dos?
        
        //Si la esquina esta mal
        if (getCornerPosition(1) != 19 && getCornerPosition(1) != 17 && getCornerPosition(1) != 15 && getCornerPosition(1) != 13 && getCornerPosition(1) != 1)
        {
            //Acomodar esquina
            //Si la esquina no est? acomodada, s?lo puede haber 3 ubicaciones posibles, que son 3, 5 y 7
            if (getCornerPosition(1) == 3)
            {
                R(); U();
                //Para no deshacer un arista ya acomodada
                if (getEdgePosition(1) == 16)
                    U();
                RP();
            }
            if (getCornerPosition(1) == 5)
            {
                B(); U();
                //Para no deshacer un arista ya acomodada
                if (getEdgePosition(1) == 18)
                    U();
                BP();
            }
            if (getCornerPosition(1) == 7)
            {
                BP(); U();
                //Para no deshacer un arista ya acomodada
                if (getEdgePosition(1) == 18)
                    U();
                B();
            }
        }
        //Si el arista esta mal
        if (getEdgePosition(1) != 20 && getEdgePosition(1) != 18 && getEdgePosition(1) != 16 && getEdgePosition(1) != 14 && getEdgePosition(1) != 9)
        {
            //Acomodar arista
            //Si el arista no est? acomodada, solo puede haber 10, 11 y 12
            if (getEdgePosition(1) == 10)
            {
                R(); U();
                //Para no deshacer una esquina ya acomodada
                if (getCornerPosition(1) == 15)
                    U();
                RP();
            }
            if (getEdgePosition(1) == 11)
            {
                B(); U();
                if (getCornerPosition(1) == 17)
                    U();
                BP();
            }
            if (getEdgePosition(1) == 12)
            {
                BP(); U();
                if (getCornerPosition(1) == 19)
                    U();
                B();
            }
        }

    }
}

void setBlock2() //Orienta bloque 2 (esquina y arista)
{
    //Se repite la instrucci?n hasta que el bloque est? acomodado para aplicar alg?n algoritmo
    while ((getCornerPosition(2) != 19 && getCornerPosition(2) != 17 && getCornerPosition(2) != 15 && getCornerPosition(2) != 13 && getCornerPosition(2) != 3) ||
        (getEdgePosition(2) != 20 && getEdgePosition(2) != 18 && getEdgePosition(2) != 16 && getEdgePosition(2) != 14 && getEdgePosition(2) != 10))
    {
        //Si el bloque no est? acomodado, acomodarlo
        //?Qu? es lo que no est? acomodaado? ?La esquina? ?El arista? ?Los dos?
        //Si la esquina esta mal
        if (getCornerPosition(2) != 19 && getCornerPosition(2) != 17 && getCornerPosition(2) != 15 && getCornerPosition(2) != 13 && getCornerPosition(2) != 3)
        {
            //Acomodar esquina
            //Si la esquina no est? acomodada, s?lo puede haber 3 ubicaciones posibles, que son 1, 5 y 7
            if (getCornerPosition(2) == 1)
            {
                F(); U();
                if (getEdgePosition(2) == 14)
                    U();
                FP();
            }
            if (getCornerPosition(2) == 5)
            {
                B(); U();
                if (getEdgePosition(2) == 18)
                    U();
                BP();
            }
            if (getCornerPosition(2) == 7)
            {
                BP(); U();
                if (getEdgePosition(2) == 18)
                    U();
                B();
            }
        }
        //Si el arista esta mal
        if (getEdgePosition(2) != 20 && getEdgePosition(2) != 18 && getEdgePosition(2) != 16 && getEdgePosition(2) != 14 && getEdgePosition(2) != 10)
        {
            //Acomodar arista
            //Si el arista no est? acomodada, solo puede haber 9, 11 y 12
            if (getEdgePosition(2) == 9)
            {
                F(); U();
                if (getCornerPosition(2) == 13)
                    U();
                FP();
            }
            if (getEdgePosition(2) == 11)
            {
                B(); U();
                if (getCornerPosition(2) == 17)
                    U();
                BP();
            }
            if (getEdgePosition(2) == 12)
            {
                BP(); U();
                if (getCornerPosition(2) == 19)
                    U();
                B();
            }
        }
    }
}

void setBlock3() //Orienta bloque 3 (esquina y arista)
{
    //Se repite la instrucci?n hasta que el bloque est? acomodado para aplicar alg?n algoritmo
    while ((getCornerPosition(3) != 19 && getCornerPosition(3) != 17 && getCornerPosition(3) != 15 && getCornerPosition(3) != 13 && getCornerPosition(3) != 5) ||
        (getEdgePosition(3) != 20 && getEdgePosition(3) != 18 && getEdgePosition(3) != 16 && getEdgePosition(3) != 14 && getEdgePosition(3) != 11))
    {
        //Si el bloque no est? acomodado, acomodarlo
        //?Qu? es lo que no est? acomodaado? ?La esquina? ?El arista? ?Los dos?
        //Si la esquina esta mal
        if (getCornerPosition(3) != 19 && getCornerPosition(3) != 17 && getCornerPosition(3) != 15 && getCornerPosition(3) != 13 && getCornerPosition(3) != 5)
        {
            //Acomodar esquina
            //Si la esquina no est? acomodada, s?lo puede haber 3 ubicaciones posibles, que son 1,3 y 7
            if (getCornerPosition(3) == 1)
            {
                F(); U();
                if (getEdgePosition(3) == 14)
                    U();
                FP();
            }
            if (getCornerPosition(3) == 3)
            {
                R(); U();
                if (getEdgePosition(3) == 16)
                    U();
                RP();
            }
            if (getCornerPosition(3) == 7)
            {
                BP(); U();
                if (getEdgePosition(3) == 18)
                    U();
                B();
            }
        }
        //Si el arista est? mal
        if (getEdgePosition(3) != 20 && getEdgePosition(3) != 18 && getEdgePosition(3) != 16 && getEdgePosition(3) != 14 && getEdgePosition(3) != 11)
        {
            //Acomodar Arista
            //Si el arista no est? acomodado, solo puede haber 3 ubicaciones posibles que son 9,10 y 12
            if (getEdgePosition(3) == 9)
            {
                F(); U();
                if (getCornerPosition(3) == 13)
                    U();
                FP();
            }
            if (getEdgePosition(3) == 10)
            {
                R(); U();
                if (getCornerPosition(3) == 15)
                    U();
                RP();
            }
            if (getEdgePosition(3) == 12)
            {
                BP(); U();
                if (getCornerPosition(3) == 19)
                    U();
                B();
            }
        }
    }
}

void setBlock4() //Orienta bloque 4 (esquina y arista)
{
    //Se repite la instrucci?n hasta que el bloque est? acomodado para aplicar alg?n algoritmo
    while ((getCornerPosition(4) != 19 && getCornerPosition(4) != 17 && getCornerPosition(4) != 15 && getCornerPosition(4) != 13 && getCornerPosition(4) != 7) ||
        (getEdgePosition(4) != 20 && getEdgePosition(4) != 18 && getEdgePosition(4) != 16 && getEdgePosition(4) != 14 && getEdgePosition(4) != 12))
    {
        //Si el bloque no est? acomodado, acomodarlo
        //?Qu? es lo que no est? acomodaado? ?La esquina? ?El arista? ?Los dos?
        //Si la esquina esta mal
        if (getCornerPosition(4) != 19 && getCornerPosition(4) != 17 && getCornerPosition(4) != 15 && getCornerPosition(4) != 13 && getCornerPosition(4) != 7)
        {
            //Acomodar esquina
            //Si la esquina no esta acomodada solo puede haber 3 ubicaciones posibles, 1,3 y 5
            if (getCornerPosition(4) == 1)
            {
                F(); U();
                if (getEdgePosition(4) == 14)
                    U();
                FP();
            }
            if (getCornerPosition(4) == 3)
            {
                R(); U();
                if (getEdgePosition(4) == 16)
                    U();
                RP();
            }
            if (getCornerPosition(4) == 5)
            {
                B(); U();
                if (getEdgePosition(4) == 18)
                    U();
                BP();
            }
        }
        //Si el arista esta mal
        if (getEdgePosition(4) != 20 && getEdgePosition(4) != 18 && getEdgePosition(4) != 16 && getEdgePosition(4) != 14 && getEdgePosition(4) != 12)
        {
            //Acomodar arista
            //Ubicaciones posibles 9, 10 y 11
            if (getEdgePosition(4) == 9)
            {
                F(); U();
                if (getCornerPosition(4) == 13)
                    U();
                FP();
            }
            if (getEdgePosition(4) == 10)
            {
                R(); U();
                if (getCornerPosition(4) == 15)
                    U();
                RP();
            }
            if (getEdgePosition(4) == 11)
            {
                B(); U();
                if (getCornerPosition(4) == 17)
                    U();
                BP();
            }
        }
    }
}

//Resolver bloque 1
void solveBlock1()
{
    int frontColor = front[1][1];
    int leftColor = left[1][1];
    int downColor = down[1][1];
    int esquinaPos = 0;
    int aristaPos = 0;

    //Mientras el bloque 1 no esta acomodado
    while ((front[1][0] != frontColor) || (front[2][0] != frontColor) || (left[1][2] != leftColor) || (left[2][2] != leftColor)) 
	{
        esquinaPos = getCornerPosition(1);
        aristaPos = getEdgePosition(1);

        //Acomodar el bloque

        //-------------------------------------------------------------------------------------------SI LA ESQUINA Y EL ARISTA ESTAN ANCLADOS
        if (esquinaPos == 1 && aristaPos == 9) {
        	
            //Cuando la esquina y el arista estan anclados, hay 5 Algoritmos posibles
            //Si el arista esta bien Colocada (2 Algoritmos)
            if (front[1][0] == frontColor && left[1][2] == leftColor)
            {
                //DownColor en cara frontal
                if (front[2][0] == downColor)
                {
                    F(); UP(); FP(); U(); F(); U(); U(); FP(); U(); F(); UP(); FP(); return; // *WORKING* //
                }
                //DownColor en cara izquierda
                else
                {
                    F(); U(); FP(); UP(); F(); U(); U(); FP(); UP(); F(); U(); FP(); return; // *WORKING* //
                }
            }
            //Si el arista no esta bien acomodada (3 algoritmos)
            if (front[1][0] != frontColor && left[1][2] != leftColor)
            {
                //Si la esquina esta bien acomodada
                if (down[0][0] == downColor)
                {
                    F(); U(); U(); FP(); U(); F(); U(); U(); FP(); U(); LP(); UP(); L(); return; // *WORKING* //
                }
                //Si downColor esta en la cara frontal
                else if (front[2][0] == downColor)
                {
                    F(); UP(); FP(); U(); FP(); UP(); F(); LP(); U(); L(); return; // *WORKING* //
                }
                //Si downColor esta en la cara izquierda
                else
                {
                    LP(); U(); L(); U(); U(); F(); U(); FP(); U(); F(); UP(); FP(); return; // *WORKING* //
                }
            }
        }
        //-------------------------------------------------------------------------------------------SI SOLO LA ESQUINA ESTA ANCLADA
        else if (esquinaPos == 1 && aristaPos != 9) {
        	
            //Si solo la esquina esta anclada, hay 6 algoritmos posibles
            //Si la esquina esta bien colocada (2 algoritmos)
            if (down[0][0] == downColor)
            {
                //Orientar arista a posicion 20
                while (getEdgePosition(1) != 20)
                {
                    UP();
                }
                //Completa la cara izquierda?
                if (left[0][1] == leftColor)
                {
                    U(); F(); UP(); FP(); UP(); LP(); U(); L(); return; // *WORKING* //
                }
                else
                {
                    UP(); UP(); LP(); U(); L(); U(); F(); UP(); FP(); return; // *WORKING* //
                }
            }
            //Si downColor esta en la cara frontal (2 algoritmos)
            if (front[2][0] == downColor)
            {
                //Orientar arista a posicion 20
                while (getEdgePosition(1) != 20)
                {
                    UP();
                }
                //Completa la cara izquierda?
                if (left[0][1] == leftColor)
                {
                    LP(); U(); L(); UP(); LP(); U(); L(); return; // *WORKING* //
                }
                else
                {
                    UP(); F(); U(); FP(); UP(); F(); U(); FP(); return; // *WORKING* //
                }
            }
            //Si downColor esta en la cara izquierda (2 algoritmos)
            if (left[2][2] == downColor)
            {
                //Orientar arista a posicion 14
                while (getEdgePosition(1) != 14)
                {
                    U();
                }
                //Completa la cara frontal?
                if (front[0][1] == frontColor)
                {
                    F(); UP(); FP(); U(); F(); UP(); FP(); return; // *WORKING* //
                }
                else
                {
                    UP(); F(); UP(); FP(); LP(); UP(); L(); return; // *WORKING* //
                }
            }
        }
        //---------------------------------------------------------------------------------------------SI SOLO EL ARISTA ESTA ANCLADA
        else if (esquinaPos != 1 && aristaPos == 9) {
            //Si SOLO la arista esta bien colocada, hay 6 algoritmos posibles
            //Orientar esquina a posicion 13
            while (getCornerPosition(1) != 13)
            {
                U();
            }
            //Si la arista esta bien colocada (3 algoritmos)
            if (front[1][0] == frontColor && left[1][2] == leftColor)
            {
			   //Si downColor esta en la cara superior
			   if (up[2][0] == downColor)
			   {
					F(); U(); FP(); UP(); F(); U(); FP(); UP(); F(); U(); FP(); return; // *WORKING* //
			   }
			   //Si downColor esta en frontal
			   if (front[0][0] == downColor)
			   {
			   		UP(); F(); U(); U(); FP(); U(); F(); U(); FP(); return; // *WORKING* //
			   }
			   //Si downColor est? en la cara izquierda
			   if (left[0][2] == downColor)
			   {
			   		UP(); F(); UP(); FP(); U(); U(); F(); UP(); FP(); return; // *WORKING* //
			   } 
            }
            //Si la arista NO esta bien colocada (3 algoritmos)
            else
            {
            	//Si downColor esta en la cara superior
            	if (up[2][0] == downColor)
            	{
            		F(); UP(); FP(); LP(); U(); U(); L(); return; // *WORKING* //
				}
            	//Si downColor esta en la cara frontal
            	if (front[0][0] == downColor)
            	{
            		U(); LP(); UP(); L(); UP(); F(); U(); FP(); return; // *WORKING* // 
				}
            	//Si downColor esta en la cara izquierda
            	if (left[0][2] == downColor)
            	{
            		UP(); F(); U(); FP(); U(); LP(); UP(); L(); return; // *WORKING* //
				}
            }
        }
        //---------------------------------------------------------------------------------------------NINGUNA PIEZA ESTA ANCLADA
        else {
			//Si ninguna esta anclada, pueden estar juntas o separadas
			//Orientar esquina a posicion 13
			while (getCornerPosition(1) != 13)
			{
				U();
			}
			
			//---------------------------------------------Si la piezas estan Juntas (12 Algoritmos)
			
			//Si estan juntas hacia frontal (6 Algoritmos)
			if (getEdgePosition(1) == 14)
			{
				//Si downColor esta en la cara superior (2 algoritmos)
				if (up[2][0] == downColor)
				{
					//El arista completa la cara?
					if (front[0][1] == frontColor)
					{
						F(); U(); U(); FP(); UP(); F(); U(); FP(); return; // *WORKING* //
					}
					else
					{
						F(); U(); FP(); U(); F(); U(); U(); FP(); LP(); U(); U(); L(); return; // *WORKING* //
					}
				}
				//Si downColor esta en la cara frontal (2 algoritmos)
				if (front[0][0] == downColor)
				{
					//El arista completa la cara frontal?
					if (front[0][1] == frontColor)
					{
						UP(); F(); UP(); FP(); U(); F(); U(); FP(); return; // *WORKING* //
					}
					else
					{
						F(); UP(); FP(); U(); U(); LP(); UP(); L(); return; // *WORKING* //
					}
				}
				//Si downColor esta en la cara izquierda (2 algoritmos)	
				if (left[0][2] == downColor)
				{
					//El arista completa la cara frotal?
					if (front[0][1] == frontColor)
					{
						U(); F(); UP(); FP(); return;  // *WORKING* //
					}
					else 
					{
						UP(); F(); U(); U(); FP(); U(); LP(); UP(); L(); return; // *WORKING* //
					}
				}
			}
			//Si estan juntas hacia izquierda (6 algoritmos)
			if (getEdgePosition(1) == 20)
			{
				//Si downColor esta en la cara superior (2 algoritmos)
				if (up[2][0] == downColor)
				{
					//El arista completa la cara?
					if (left[0][1] == leftColor)
					{
						F(); U(); U(); FP(); UP(); F(); U(); FP(); return; // *WORKING* //
					}
					else
					{
						F(); U(); FP(); U(); U(); F(); U(); FP(); UP(); F(); U(); FP(); return; // *WORKING* //
					}
				}
				//Si downColor esta en la cara frontal (2 algoritmos)
				if (front[0][0] == downColor)
				{
					//El arista completa la cara?
					if (left[0][1] == leftColor)
					{
						UP(); LP(); U(); L(); return; // *WORKING* //
					}
					else
					{
						U(); LP(); U(); U(); L(); UP(); F(); U(); FP(); return; // *WORKING* //
					}
				}
				//Si downColor esta en la cara izquierda (2 algoritmos)
				if (left[0][2] == downColor)
				{
					//El arista completa la cara?
					if (left[0][1] == leftColor)
					{
						U(); LP(); U(); L(); UP(); LP(); UP(); L(); return;	// *WORKING* //
					}
					else
					{
						LP(); U(); L(); U(); U(); F(); U(); FP(); return; // *WORKING* //
					}
				}
			}
				
			//-----------------------------------------------Si las piezas estan Separadas	(12 Algoritmos)
			//Si estan separadas hacia derecha (6 Algoritmos)
			if (getEdgePosition(1) == 16)
			{
				//Si downColor esta en la cara superior (2 Algoritmos)
				if (up[2][0] == downColor)
				{
					//Color frontal
					if (up[1][2] == frontColor)
					{
						U(); U(); LP(); UP(); L(); UP(); LP(); U(); L(); return; // *WORKING* //
					}
					else
					{
						U(); F(); U(); U(); FP(); U(); F(); UP(); FP(); return; // *WORKING* //
					}
				}
				//Si downColor esta en la cara frontal (2 algoritmos)
				if (front[0][0] == downColor)
				{
					//Color frontal
					if (up[1][2] == frontColor)
					{
						U(); LP(); U(); U(); L(); U(); U(); LP(); U(); L(); return;	// *WORKING* //
					}
					else
					{
						F(); U(); FP(); return; // *WORKING* //
					}
				}
				//Si downColor esta en la cara izquierda (2 algoritmos)
				if (left[0][2] == downColor)
				{
					//Color frontal
					if (up[1][2] == frontColor)
					{
						UP(); F(); UP(); FP(); U(); LP(); UP(); L(); return; // *WORKING* //
					}
					else
					{
						UP(); F(); U(); FP(); UP(); F(); U(); U(); FP(); return; // *WORKING* //
					}
				}
			}
			//Si estan separadas hacia trasero (6 algoritmos)
			if (getEdgePosition(1) == 18)
			{
				//Si downColor esta en la cara superior (2 algoritmos)
				if (up[2][0] == downColor)
				{
					//Color frontal
					if (up[0][1] == frontColor)
					{
						UP(); LP(); U(); U(); L(); UP(); LP(); U(); L(); return; // *WORKING* //
					}
					else
					{
						U(); U(); F(); U(); FP(); U(); F(); UP(); FP(); return;  // *WORKING* //
					}
				}
				//Si downColor esta en la cara frontal (2 algoritmos)
				if (front[0][0] == downColor)
				{
					//Color frontal
					if (up[0][1] == frontColor)
					{
						U(); LP(); UP(); L(); U(); U(); LP(); U(); L(); return;  // *WORKING* //
					}
					else
					{
						UP(); F(); U(); FP(); U(); F(); U(); FP(); return; // *WORKING* //
					}
				}
				//Si downColor esta en la cara izquierda (2 algoritmos)
				if (left[0][2] == downColor)
				{
					//Color frontal
					if (up[0][1] == frontColor)
					{
						LP(); UP(); L(); return; // *WORKING* //
					}
					else
					{
						UP(); F(); U(); U(); FP(); U(); U(); F(); UP(); FP(); return; // *WORKING* //
					}
				}
			}
		}
    }
}

//Resolver bloque 2
void solveBlock2()
{
	int frontColor = front[1][1];
	int rightColor = right[1][1];
	int downColor = down[1][1];
	int esquinaPos = 0;
	int aristaPos = 0;
	
	//Mientras el bloque 2 no este acomodado
	while ((front[1][2] != frontColor) || (front[2][2] != frontColor) || (right[1][0] != rightColor) || (right[2][0] != rightColor)) 
	{
		esquinaPos = getCornerPosition(2);
		aristaPos = getEdgePosition(2);
		
		//Acomodar el bloque
		
		//-------------------------------------------------------------------------------------------SI LA ESQUINA Y EL ARISTA ESTAN ANCLADOS
		if (esquinaPos == 3 && aristaPos == 10) 
		{
			
			//Aqui hay 5 algoritmos posibles
			
			// [MAIN] El arista SI esta bien colocada (2)
			if (front[1][2] == frontColor && right[1][0] == rightColor)
			{
				//DownColor esta en la cara frontal
				if (front[2][2] == downColor)
				{
					R(); U(); RP(); UP(); R(); U(); U(); RP(); UP(); R(); U(); RP(); return; // *WORKING* //
				}
				//DownColor esta en la cara derecha
				else
				{
					R(); UP(); RP(); U(); R(); U(); U(); RP(); U(); R(); UP(); RP(); return; // *WORKING* //
				}
			}
			// [MAIN] El arista NO esta bien colocada (3)
			if (front[1][2] != frontColor && right[1][0] != rightColor)
			{
				//DownColor esta en la cara inferior (esquina bien colocada)
				if (down[0][2] == downColor)
				{
					R(); U(); U(); RP(); U(); R(); U(); U(); RP(); U(); FP(); UP(); F(); return; // *WORKING* //
				}
				//DownColor esta en la cara frontal
				if (front[2][2] == downColor)
				{
					FP(); U(); F(); U(); U(); R(); U(); RP(); U(); R(); UP(); RP(); return; // *WORKING* //
				}
				//DownColor esta en la cara derecha
				if (right[2][0] == downColor)
				{
					R(); UP(); RP(); U(); RP(); UP(); R(); FP(); U(); F(); return; // *WORKING* //
				}
			}
			
		}
		
		//-------------------------------------------------------------------------------------------SI SOLO LA ESQUINA ESTA ANCLADA
		else if (esquinaPos == 3 && aristaPos != 10) 
		{
			//Aqui hay 6 algoritmos posibles
			
			// [MAIN] La esquina esta bien colocada (2)
			if (down[0][2] == downColor)
			{
				//Orientar arista a posicion 14
				while (getEdgePosition(2) != 14)
				{
					UP();
				}
				//Completa la cara FRONTAL
				if (front[0][1] == frontColor)
				{
					U(); R(); UP(); RP(); UP(); FP(); U(); F(); return; // *WORKING* //
				}
				//Completa la cara DERECHA
				else
				{
					UP(); UP(); FP(); U(); F(); U(); R(); UP(); RP(); return; // *WORKING* //
				}
			}
			// [MAIN] DownColor esta en la cara FRONTAL (2)
			if (front[2][2] == downColor)
			{
				//Orientar arista a posicion 16
				while (getEdgePosition(2) != 16)
				{
					UP();
				}
				//Completa la cara DERECHA
				if (right[0][1] == rightColor)
				{
					R(); UP(); RP(); U(); R(); UP(); RP(); return; // *WORKING* //
				}
				//NO Completa la cara DERECHA
				else
				{
					UP(); R(); UP(); RP(); FP(); UP(); F(); return; // *WORKING* //
				}
			}
			// [MAIN] DownColor esta en la cara DERECHA (2)
			if (right[2][0] == downColor)
			{
				//Orientar arista a posicion 14
				while (getEdgePosition(2) != 14)
				{
					UP();
				}
				//Completa la cara FRONTAL
				if (front[0][1] == frontColor)
				{
					 FP(); U(); F(); UP(); FP(); U(); F(); return; // *WORKING* //
				}
				//NO Completa la cara FRONTAL
				else
				{
					UP(); R(); U(); RP(); UP(); R(); U(); RP(); return; // *WORKING* //
				}
			}
		}
		
		//-------------------------------------------------------------------------------------------SI SOLO EL ARISTA ESTA ANCLADA
		else if (esquinaPos != 3 && aristaPos == 10) 
		{
			
			//Aqui hay 6 algoritmos posibles
			//Orientar esquina a posicion 15
			while (getCornerPosition(2) != 15)
			{
				U();
			}
			
			// [MAIN] El arista SI esta bien colocada (3)
			if (front[1][2] == frontColor && right[1][0] == rightColor)
			{
				//DownColor esta en la cara SUPERIOR
				if (up[2][2] == downColor)
				{
					R(); U(); RP(); UP(); R(); U(); RP(); UP(); R(); U(); RP(); return; // *WORKING* //
				}
				//DownColor esta en la cara FRONTAL
				if (front[0][2] == downColor)
				{
					UP(); R(); UP(); RP(); U(); U(); R(); UP(); RP(); return; // *WORKING* //
				}
				//DownColor esta en la cara DERECHA
				if (right[0][0] == downColor)
				{
					UP(); R(); U(); U(); RP(); U(); R(); U(); RP(); return; // *WORKING* //
				}
			}
			// [MAIN] El arista NO esta bien colocada (3)
			else
			{
				//DownColor esta en la cara SUPERIOR
				if (up[2][2] == downColor)
				{
					R(); UP(); RP(); FP(); U(); U(); F(); return; // *WORKING* //
				}
				//DownColor esta en la cara FRONTAL
				if (front[0][2] == downColor)
				{
					UP(); R(); U(); RP(); U(); FP(); UP(); F(); return; // *WORKING* //
				}
				//DownColor esta en la cara DERECHA
				if (right[0][0] == downColor)
				{
					U(); FP(); UP(); F(); UP(); R(); U(); RP(); return; // *WORKING* //
				}
			}
		}
		
		//-------------------------------------------------------------------------------------------NINGUNA PIEZA ESTA ANCLADA
		else 
		{
			//Aqui hay 24 algoritmos posibles
			//Orientar esquina a posicion 15
			while (getCornerPosition(2) != 15)
			{
				UP();
			}
			
			// [MAIN] Las piezas estan JUNTAS (12)
			if (getEdgePosition(2) == 14 || getEdgePosition(2) == 16)
			{
				// [O] Las piezas estan JUNTAS HACIA FRONTAL (6)
				if (getEdgePosition(2) == 14)
				{
					//DownColor esta en la cara SUPERIOR (2)
					if (up[2][2] == downColor)
					{
						//El arista completa la cara FRONTAL
						if (front[0][1] == frontColor)
						{
							FP(); U(); U(); F(); U(); FP(); UP(); F(); return; // *WORKING* //
						}
						//El arista NO completa la cara FRONTAL
						else
						{
							R(); U(); RP(); U(); U(); R(); U(); RP(); UP(); R(); U(); RP(); return; // *WORKING* //
						}
					}
					//DownColor esta en la cara FRONTAL (2)
					if (front[0][2] == downColor)
					{
						//La arista completa la cara FRONTAL
						if (front[0][1] == frontColor)
						{
							U(); FP(); U(); F(); UP(); FP(); UP(); F(); return; // *WORKING* //
						}
						//La arista NO completa la cara FRONTAL
						else
						{
							FP(); U(); F(); U(); U(); R(); U(); RP(); return; // *WORKING* //
						}
					}
					//DownColor esta en la cara DERECHA (2)
					if (right[0][0] == rightColor)
					{
						//La arista completa la cara FRONTAL
						if (front[0][1] == frontColor)
						{
							UP(); FP(); U(); F(); return; // *WORKING* //
						}
						//La arista NO completa la cara FRONTAL
						else
						{
							U(); FP(); U(); U(); F(); UP(); R(); U(); RP(); return; // *WORKING* //
						}
					}
				}
				// [O] las piezas estan JUNTAS HACIA DERECHA (6)
				if (getEdgePosition(2) == 16)
				{
					//DownColor esta en la cara SUPERIOR (2)
					if (up[2][2] == downColor)
					{
						//La arista completa la cara DERECHA
						if (right[0][1] == rightColor)
						{
							R(); U(); U(); RP(); UP(); R(); U(); RP(); return; // *WORKING* //
						}
						//La arista NO completa la cara DERECHA
						else
						{
							R(); U(); RP(); U(); R(); U(); U(); RP(); FP(); U(); U(); F(); return; // *WORKING* //
						}
					}
					//DownColor esta en la cara FRONTAL (2)
					if (front[0][2] == downColor)
					{
						//La arista completa la cara DERECHA
						if (right[0][1] == rightColor)
						{
							U(); R(); UP(); RP(); return; // *WORKING* //
						}
						//La arista NO completa la cara DERECHA
						else
						{
							UP(); R(); U(); U(); RP(); U(); FP(); UP(); F(); return; // *WORKING* //
						}
					}
					//DownColor esta en la cara DERECHA (2)
					if (right[0][0] == downColor)
					{
						//La arista completa la cara DERECHA
						if (right[0][1] == rightColor)
						{
							UP(); R(); UP(); RP(); U(); R(); U(); RP(); return; // *WORKING* //
						}
						//La arista NO completa la cara DERECHA
						else
						{
							R(); UP(); RP(); U(); U(); FP(); UP(); F(); return; // *WORKING* //
						}
					}
				}
			}
			
			// [MAIN] las piezas estan SEPARADAS (12)
			else
			{
				// [I I] Las piezas estan SEPARADAS HACIA TRASERA (6)
				if (getEdgePosition(2) == 18)
				{
					//Esquina. DownColor esta en la cara SUPERIOR (2)
					if (up[2][2] == downColor)
					{
						//Arista. Color FRONTAL
						if (up[0][1] == frontColor)
						{
							U(); R(); U(); U(); RP(); U(); R(); UP(); RP(); return; // *WORKING* //
						}
						//Arista. Color DERECHA
						else
						{
							U(); U(); FP(); UP(); F(); UP(); FP(); U(); F(); return; // *WORKING* //
						}
					}
					//Esquina. DownColor esta en la cara FRONTAL (2)
					if (front[0][2] == downColor)
					{
						//Arista. Color FRONTAL
						if (up[0][1] == frontColor)
						{
							UP(); R(); U(); RP(); UP(); R(); U(); U(); RP(); return; // *WORKING* // 
						}
						//Arista. Color DERECHA
						else
						{
							UP(); R(); UP(); RP(); U(); FP(); UP(); F(); return; // *WORKING* //
						}
					}
					//Esquina. DownColor esta en la cara DERECHA (2)
					if (right[0][0] == downColor)
					{
						//Arista. Color FRONTAL
						if (up[0][1] == frontColor)
						{
							R(); U(); RP(); return; // *WORKING* //
						}
						//Arista. Color DERECHA
						else
						{
							U(); FP(); U(); U(); F(); U(); U(); FP(); U(); F(); return; // *WORKING* //
						}
					}
				}
				// [I I] Las piezas estan SEPARADAS HACIA IZQUIERDA (6)
				if (getEdgePosition(2) == 20)
				{
					//Esquina. DownColor en cara SUPERIOR (2)
					if (up[2][2] == downColor)
					{
						//Arista. Color FRONTAL
						if (up[1][0] == frontColor)
						{
							U(); U(); R(); U(); RP(); U(); R(); UP(); RP(); return; // *WORKING* //
						}
						//Arista. Color DERECHA
						else
						{
							UP(); FP(); U(); U(); F(); UP(); FP(); U(); F(); return; // *WORKING* //
						}
					}
					//Esquina. DownColor en cara FRONTAL (2)
					if (front[0][2] == downColor)
					{
						//Arista. Color FRONTAL
						if (up[1][0] == frontColor)
						{
							UP(); R(); U(); U(); RP(); U(); U(); R(); UP(); RP(); return; // *WORKING* //
						}
						//Arista. Color DERECHA
						else
						{
							FP(); UP(); F(); return; // *WORKING* //
						}
					}
					//Esquina. DownColor en cara DERECHA (2)
					if (right[0][0] == downColor)
					{
						//Arista. Color FRONTAL
						if (up[1][0] == frontColor)
						{
							UP(); R(); U(); RP(); U(); R(); U(); RP(); return; // *WORKING* //
						}
						//Arista. Color DERECHA
						else
						{
							U(); FP(); UP(); F(); U(); U(); FP(); U(); F(); return; // *WORKING* //
						}
					}
				}
			}
				
		}
	}
}

//Resolver bloque 3
void solveBlock3()
{
	int backColor = back[1][1];
	int rightColor = right[1][1];
	int downColor = down[1][1];
	int esquinaPos = 0;
	int aristaPos = 0;
	
	//Mientras el bloque 3 no este acomodado
	while ((right[1][2] != rightColor) || (right[2][2] != rightColor) || (back[1][0] != backColor) || (back[2][0] != backColor))
	{
		esquinaPos = getCornerPosition(3);
		aristaPos = getEdgePosition(3);
		
		//Acomodar el bloque
		
		//-------------------------------------------------------------------------------------------SI LA ESQUINA Y EL ARISTA ESTAN ANCLADOS
		if (esquinaPos == 5 && aristaPos == 11)
		{
			//Aqui hay 5 algoritmos posibles
			// [MAIN] El arista SI esta bien colocada (2)
			if (right[1][0] == rightColor && back[1][0] == backColor)
			{
				//DownColor esta en la cara DERECHA
				if (right[2][2] == downColor)
				{
					B(); U(); BP(); UP(); B(); U(); U(); BP(); UP(); B(); U(); BP(); return; // *WORKING* //
				}
				//DownColor esta en la cara TRASERA
				else
				{
					B(); UP(); BP(); U(); B(); U(); U(); BP(); U(); B(); UP(); BP(); return; // *WORKING* //
				}
			}
			// [MAIN] El arista NO esta bien colocada (3)
			else
			{
				//DownColor esta en la cara INFERIOR (esquina bien colocada)
				if (down[2][2] == downColor)
				{
					B(); U(); U(); BP(); U(); B(); U(); U(); BP(); U(); RP(); UP(); R(); return; // *WORKING* //
				}
				//DownColor esta en la cara DERECHA
				if (right[2][2] == rightColor)
				{
					RP(); U(); R(); U(); U(); B(); U(); BP(); U(); B(); UP(); BP(); return; // *WORKING* //
				} 
				//DownColor esta en la cara TRASERA
				if (back[2][0] == backColor)
				{
					B(); UP(); BP(); U(); BP(); UP(); B(); RP(); U(); R(); return; // *WORKING* //
				}
			}
		}
		//-------------------------------------------------------------------------------------------SI SOLO LA ESQUINA ESTA ANCLADA (R = B) (F = R)
		else if (esquinaPos == 5 && aristaPos != 11) 
		{
			//Aqui hay 6 algoritmos posibles
			
			// [MAIN] La esquina esta bien colocada (2)
			if (down[2][2] == downColor)
			{
				//Orientar arista a posicion 16
				while (getEdgePosition(3) != 16)
				{
					UP();
				}
				//El arista completa la cara DERECHA
				if (right[0][1] == rightColor)
				{
					U(); B(); UP(); BP(); UP(); RP(); U(); R(); return; // *WORKING* //
				}
				//El arista completa la cara TRASERA
				else
				{
					UP(); UP(); RP(); U(); R(); U(); B(); UP(); BP(); return; // *WORKING* //
				}
			}
			// [MAIN] DownColor esta en la cara DERECHA (2)
			if (right[2][2] == downColor)
			{
				//Orientar arista a posicion 18
				while (getEdgePosition(3) != 18)
				{
					UP();
				}
				//El arista completa la cara TRASERA
				if (back[0][1] == backColor)
				{
					B(); UP(); BP(); U(); B(); UP(); BP(); return; // *WORKING* //
				}
				//El arista NO completa la cara TRASERA
				else
				{
					UP(); B(); UP(); BP(); RP(); UP(); R(); return; // *WORKING* //
				}
			}
			// [MAIN] DownColor esta en la cara TRASERA (2)
			if (back[2][0] == downColor)
			{
				//Orientar arista a posicion 16
				while (getEdgePosition(3) != 16)
				{
					UP();
				}
				//El arista completa la cara DERECHA
				if (right[0][1] == rightColor)
				{
					RP(); U(); R(); UP(); RP(); U(); R(); return; // *WORKING* //
				}
				//El arista NO completa la cara DERECHA
				else
				{
					UP(); B(); U(); BP(); UP(); B(); U(); BP(); return; // *WORKING* //
				}
			}

		}
		//-------------------------------------------------------------------------------------------SI SOLO EL ARISTA ESTA ANCLADA
		else if (esquinaPos != 5 && aristaPos == 11)
		{
			//Aqui hay 6 algoritmos posibles
			//Orientar esquina a posicion 17
			while (getCornerPosition(3) != 17)
			{
				U();
			}
			// [MAIN] El arista SI esta bien colocada (3)
			if (right[1][2] == rightColor)
			{
				//DownColor en la cara SUPERIOR
				if (up[0][2] == downColor)
				{
					B(); U(); BP(); UP(); B(); U(); BP(); UP(); B(); U(); BP(); return; // *WORKING* //
				}
				//DownColor en la cara DERECHA
				if (right[0][2] == downColor)
				{
					UP(); B(); UP(); BP(); U(); U(); B(); UP(); BP(); return; // *WORKING* //
				}
				//DownColor en la cara TRASERA
				if (back[0][0] == downColor)
				{
					UP(); B(); U(); U(); BP(); U(); B(); U(); BP(); return; // *WORKING* //
				}
			}
			// [MAIN] El arista NO esta bien colocada (3)
			else
			{
				//DownColor en la cara SUPERIOR
				if (up[0][2] == downColor)
				{
					B(); UP(); BP(); RP(); U(); U(); R(); return; // *WORKING* //
				}
				//DownColor en la cara DERECHA
				if (right[0][2] == downColor)
				{
					UP(); B(); U(); BP(); U(); RP(); UP(); R(); return; // *WORKING* //
				}
				//DownColor en la cara TRASERA
				if (back[0][0] == downColor)
				{
					U(); RP(); UP(); R(); UP(); B(); U(); BP(); return; // *WORKING* //
				}
			}
		}
		//-------------------------------------------------------------------------------------------NINGUNA PIEZA ESTA ANCLADA
		else
		{
			//Aqui hay 24 algoritmos posibles
			
			//Orientar esquina a posicion 17
			while (getCornerPosition(3) != 17)
			{
				U();
			}
			// [MAIN] Las piezas estan JUNTAS (12)
			if (getEdgePosition(3) == 18 || getEdgePosition(3) == 16)
			{
				// [O] Las piezas estan JUNTAS HACIA DERECHA (6)
				if (getEdgePosition(3) == 16)
				{
					//DownColor esta en la cara SUPERIOR (2)
					if (up[0][2] == downColor)
					{
						//La arista completa la cara DERECHA
						if (right[0][1] == rightColor)
						{
							RP(); U(); U(); R(); U(); RP(); UP(); R(); return; // *WORKING* //
						}
						//La arista NO completa la cara DERECHA
						else
						{
							B(); U(); BP(); U(); U(); B(); U(); BP(); UP(); B(); U(); BP(); return; // *WORKING* //
						}
					}
					//DownColor esta en la cara DERECHA (2)
					if (right[0][2] == downColor)
					{
						//La arista completa la cara DERECHA
						if (right[0][1] == rightColor)
						{
							U(); RP(); U(); R(); UP(); RP(); UP(); R(); return; // *WORKING* //
						}
						//La arista NO completa la cara DERECHA
						else
						{
							RP(); U(); R(); U(); U(); B(); U(); BP(); return; // *WORKING* //
						}
						
					}
					//DownColor esta en la cara TRASERA (2)
					if (back[0][0] == downColor)
					{
						//La arista completa la cara DERECHA
						if (right[0][1] == rightColor)
						{
							UP(); RP(); U(); R(); return; // *WORKING* //
						}
						//La arista NO completa la cara DERECHA
						else
						{
							U(); RP(); U(); U(); R(); UP(); B(); U(); BP(); return; // *WORKING* //
						}
					}
				}
				
				// [O] Las piezas estan JUNTAS HACIA TRASERA (6)
				else
				{
					//DownColor esta en la cara SUPERIOR (2)
					if (up[0][2] == downColor)
					{
						//La arista completa la cara TRASERA
						if (back[0][1] == backColor)
						{
							B(); U(); U(); BP(); UP(); B(); U(); BP(); return; // *WORKING* //
						}
						//La arista NO completa la cara TRASERA
						else
						{
							B(); U(); BP(); U(); B(); U(); U(); BP(); RP(); U(); U(); R(); return; // *WORKING* //
						}
					}
					//DownColor esta en la cara DERECHA (2)
					if (right[0][2] == downColor)
					{
						//La arista completa la cara TRASERA
						if (back[0][1] == backColor)
						{
							U(); B(); UP(); BP(); return; // *WORKING* //
						}
						//La arista NO completa la cara TRASERA
						else
						{
							UP(); B(); U(); U(); BP(); U(); RP(); UP(); R(); return; // *WORKING* //
						}
					}
					//DownColor esta en la cara TRASERA (2)
					if (back[0][0] == downColor)
					{
						//La arista completa la cara TRASERA
						if (back[0][1] == backColor)
						{
							UP(); B(); UP(); BP(); U(); B(); U(); BP(); return; // *WORKING* //
						}
						//La arista NO completa la cara TRASERA
						else
						{
							B(); UP(); BP(); U(); U(); RP(); UP(); R(); return; // *WORKING* //
						}
					}
				}
			}
			// [MAIN] las piezas estan SEPARADAS (12)
			else
			{
				// [I I] Las piezas estan SEPARADAS HACIA FRONTAL (6)
				if (getEdgePosition(3) == 14)
				{
					//DownColor esta en la cara SUPERIOR (2)
					if (up[0][2] == downColor)
					{
						//Arista. Color DERECHA
						if (up[2][1] == rightColor)
						{
							U(); U(); B(); U(); BP(); U(); B(); UP(); BP(); return; // *WORKING* //
						}
						//Arista. Color TRASERA
						else
						{
							UP(); RP(); U(); U(); R(); UP(); RP(); U(); R(); return; // *WORKING* //
						}
					}
					//DownColor esta en la cara DERECHA (2)
					if (right[0][2] == downColor)
					{
						//Arista. Color DERECHA
						if (up[2][1] == rightColor)
						{
							UP(); B(); U(); U(); BP(); U(); U(); B(); UP(); BP(); return; // *WORKING* //
						}
						//Arista. Color TRASERA
						else
						{
							RP(); UP(); R(); return; // *WORKING* //
						}
					}
					//DownColor esta en la cara TRASERA (2)
					if (back[0][0] == downColor)
					{
						//Arista. Color DERECHA
						if (up[2][1] == rightColor)
						{
							UP(); B(); U(); BP(); U(); B(); U(); BP(); return; // *WORKING* //
						}
						//Arista. Color TRASERA
						else
						{
							U(); RP(); UP(); R(); U(); U(); RP(); U(); R(); return; // *WORKING* //
						}
					}
				}
				// [I I] Las piezas estan SEPARADAS HACIA IZQUIERDA (6)
				else
				{
					//DownColor esta en la cara SUPERIOR (2)
					if (up[0][2] == downColor)
					{
						//Arista. Color Derecha
						if (up[1][0] == rightColor)
						{
							U(); B(); U(); U(); BP(); U(); B(); UP(); BP(); return; // *WORKING* //
						}
						//Arista. Color TRASERA
						else
						{
							U(); U(); RP(); UP(); R(); UP(); RP(); U(); R(); return; // *WORKING* //
						}
					}
					//DownColor esta en la cara DERECHA (2)
					if (right[0][2] == downColor)
					{
						//Arista. Color Derecha
						if (up[1][0] == rightColor)
						{
							UP(); B(); U(); BP(); UP(); B(); U(); U(); BP(); return; // *WORKING* //
						}
						//Arista. Color TRASERA
						else
						{
							UP(); B(); UP(); BP(); U(); RP(); UP(); R(); return; // *WORKING* //
						}
					}
					//DownColor esta en la cara TRASERA (2)
					if (back[0][0] == downColor)
					{
						//Arista. Color Derecha
						if (up[1][0] == rightColor)
						{
							B(); U(); BP(); return; // *WORKING* //
						}
						//Arista. Color TRASERA
						else
						{
							U(); RP(); U(); U(); R(); U(); U(); RP(); U(); R(); return; // *WORKING* //
						}
					}
				}
			}
		}
	}
}

//Resolver bloque 4
void solveBlock4()
{
	int backColor = back[1][1];
	int leftColor = left[1][1];
	int downColor = down[1][1];
	int esquinaPos = 0;
	int aristaPos = 0;
	
	//Mientras el bloque 4 no este acomodado
	while ((back[1][2] != backColor) || (back[2][2] != backColor) || (left[1][0] != leftColor) || (left[2][0] != leftColor))
	{
		esquinaPos = getCornerPosition(4);
		aristaPos = getEdgePosition(4);
		
		//Acomodar el bloque
		
		//-------------------------------------------------------------------------------------------SI LA ESQUINA Y EL ARISTA ESTAN ANCLADOS
		if (esquinaPos == 7 && aristaPos == 12)
		{
			//Aqui hay 5 algoritmos posibles
			
			// [MAIN] El arista SI esta bien colocada (2)
			if (back[1][2] == backColor && left[1][0] == leftColor)
			{
				//DownColor esta en la cara TRASERA
				if (back[2][2] == downColor)
				{
					L(); UP(); LP(); UP(); L(); U(); LP(); UP(); L(); U(); U(); LP(); return; // *WORKING* //
				}
				//DownColor esta en la cara IZQUIERDA
				else
				{
					L(); UP(); LP(); U(); L(); U(); U(); LP(); U(); L(); UP(); LP(); return; // *WORKING* //
				}
			}
			// [MAIN] El arista NO esta bien colocada (3)
			else
			{
				//DownColor esta en la cara INFERIOR (Esquina bien colocada)
				if (down[2][0] == downColor)
				{
					L(); UP(); LP(); U(); BP(); U(); U(); B(); U(); BP(); U(); U(); B(); return; // *WORKING* //
				}
				//DownColor esta en la cara TRASERA
				if (back[2][2] == downColor)
				{
					L(); UP(); LP(); U(); BP(); UP(); B(); UP(); BP(); UP(); B(); return; // *WORKING* //
				}
				//DownColor esta en la cara IZQUIERDA
				if (left[2][0] == downColor)
				{
					L(); U(); LP(); UP(); L(); UP(); LP(); U(); U(); BP(); UP(); B(); return; // *WORKING* //
				}
			}
		}
		//-------------------------------------------------------------------------------------------SI SOLO LA ESQUINA ESTA ANCLADA
		else if (esquinaPos == 7 && aristaPos != 12)
		{
			//Aqui hay 6 algoritmos posibles
			// [MAIN] La esquina esta bien colocada (2)
			if (down[2][0] == downColor)
			{
				//Orientar arista a posicion 18
				while (getEdgePosition(4) != 18)
				{
					UP();
				}
				//La arista completa la cara TRASERA
				if (back[0][1] == backColor)
				{
					U(); L(); UP(); LP(); UP(); BP(); U(); B(); return; // *WORKING* //
				}
				//La arista completa la cara IZQUIERDA
				else
				{
					UP(); UP(); BP(); U(); B(); U(); L(); UP(); LP(); return; // *WORKING* //
				}
			}
			// [MAIN] DownColor esta en la cara TRASERA (2)
			if (back[2][2] == downColor)
			{
				//Orientar arista a posicion 18
				while (getEdgePosition(4) != 18)
				{
					UP();
				}
				//La arista completa la cara TRASERA
				if (back[0][1] == backColor)
				{
					BP(); UP(); B(); U(); BP(); UP(); B(); return; // *WORKING* //
				}
				//La arista completa la cara IZQUIERDA
				else
				{
					UP(); L(); UP(); LP(); U(); L(); UP(); LP(); return; // *WORKING* //
				}
			}
			// [MAIN] DownColor esta en la cara IZQUIERDA (2)
			if (left[2][0] == downColor)
			{
				//Orientar arista a posicion 18
				while (getEdgePosition(4) != 18)
				{
					UP();
				}
				//La arista completa la cara TRASERA
				if (back[0][1] == backColor)
				{
					BP(); U(); B(); UP(); BP(); U(); B(); return; // *WORKING* //
				}
				//La arista completa la cara IZQUIERDA
				else
				{
					UP(); L(); U(); LP(); UP(); L(); U(); LP(); return; // *WORKING* //
				}
			}
		}
		//-------------------------------------------------------------------------------------------SI SOLO EL ARISTA ESTA ANCLADA
		else if (esquinaPos != 7 && aristaPos == 12)
		{
			
		}
		//-------------------------------------------------------------------------------------------NINGUNA PIEZA ESTA ANCLADA
		else
		{
			
		}
		
	}	
}
