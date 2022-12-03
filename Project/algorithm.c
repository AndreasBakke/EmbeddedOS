///* Online C Compiler and Editor */
//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//double WL = 150.0;
//double fill, drain, outW = 0;

//int i = 0;
//int main()
//{
//    srand(time(NULL));
//    while (i<1000){
//        
//        if (i%10 == 0) {
//            if (WL <180) {
//                drain =0;
//                fill = 1+ outW*2-WL/150; //Adjust this after seeing simulation
//                if (fill<0){
//                    fill = 0;
//                }
//            }   else {
//                fill = 0;
//                drain = 3;
//            } 
//        }
//        outW = ((double) (rand() % 1500)/1000) ;
//        WL = (int) WL - outW - drain + fill;
//        i=i+1;
//        printf("Waterlevel: %.3f, outW: %.3f, fill: %.3f \n", WL, outW, fill);
//    }

//    return 0;
//}