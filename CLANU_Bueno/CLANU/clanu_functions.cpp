// T. Grenier - Clanu 2017-2018 - INSA Lyon - GE
// g++ mnist_train_lrgd.cpp common_functions.cpp -o mnist_train_lrgd -ffast-math -fopenmp -O3

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
#ifdef _OPENMP
    #include <omp.h>
#endif

#include "clanu_functions.h"
#include "common_functions.h"

using namespace std;

///////////////////////////////////////////////////////////
//           START YOUR MODIFICATIONS HERE               //
///////////////////////////////////////////////////////////

void x_Teta_tras(float **m1, float **m2, float **res, int k, int m, int n) {
    int i, j, s;
    s = 0;
    i = 0;
    res[s][i] = 0;
    //Ici, il commence la trasposition
    for (s = 0; s < m; s++) {
        for (i = 0; i <k; i++) {
            res[s][i] = 0;
            for (j = 0; j <n; j++) {
                res[s][i] = res[s][i] + m1[s][j] * m2[i][j];
            }
        }
    }

    return;
}


float Accuracy(float **Teta, float **test_X,float *test_y,int m, int n){
    float tabMoy[m];
    //float **mult_trasp=new float[m][10];
    FLOAT_TYPE **mult_trasp=nullptr;
    allocate(&mult_trasp, m,10);
    float max, tasa;
    int i,j,pos;
    j=0;
   /* for(i=0;i<m;i++){
        mult_trasp[i]=new float[10];
    }*/
    x_Teta_tras(test_X,Teta,mult_trasp,10,m,n);//With 10 being the value of the matrix to traspose which isn't common, m the other uncommon value and n the common one
    for(i=0;i<m;i++){
        max=g(mult_trasp[i][j]);
        for(j=0;j<10;j++){
            mult_trasp[i][j]=g(mult_trasp[i][j]);
            if(mult_trasp[i][j]>max){
                max=mult_trasp[i][j];
                pos=j;
            }
        }
        tabMoy[i]=pos;
    }
    tasa=0;
    for(i=0;i<m;i++){
        if(tabMoy[i]==test_y[i])
            tasa++;
    }
    tasa=tasa/m;
    return tasa;
}


void save_teta(char *c,float **Teta, int k, int n){
    int i,j;
    ofstream ficheroSalida;
    ficheroSalida.open(c);
    for (i=0;i<k;i++){
        for(j=0;j<n;j++){
            ficheroSalida << Teta[i][j];
            if(j==n-1){
                ficheroSalida<<"\n";
            }
            else{
                ficheroSalida<<",";
            }
        }
    }
    ficheroSalida.close();
    return;
}

void open_teta(char *c, float **Teta, int k, int n){
unsigned int a,b;
        a=k;
        b=n;
 loadCSV_to_matrix( c, &Teta, &a, &b);
 /*cout<<"Teta=";
 for(unsigned int i=0; i<k; ++i) //Third, split each line
     {

     for(unsigned int j=0; j<n; j++){
     cout<<Teta[k][n];
     }
     cout<<endl;
         }*/
     }


void test_ac(float **Teta, float **test_X,float *test_y,int m, int n, char *c){
    ofstream ficheroSalida;
    int valy;
    float tabMoy[m];
    float val0, val1, val2, val3, val4, val5, val6, val7, val8, val9, cont0, cont1, cont2, cont3, cont4, cont5, cont6, cont7, cont8, cont9;
    val0=0;
    val1=0;
    val2=0;
    val3=0;
    val4=0;
    val5=0;
    val6=0;
    val7=0;
    val8=0;
    val9=0;
    cont0=0;
    cont1=0;
    cont2=0;
    cont3=0;
    cont4=0;
    cont5=0;
    cont6=0;
    cont7=0;
    cont8=0;
    cont9=0;
     ficheroSalida.open(c);
    //float **mult_trasp=new float[m][10];
    FLOAT_TYPE **mult_trasp=nullptr;
    allocate(&mult_trasp, m,10);
    float max;
    int i,j,pos;
    j=0;
   /* for(i=0;i<m;i++){
        mult_trasp[i]=new float[10];
    }*/
    x_Teta_tras(test_X,Teta,mult_trasp,10,m,n);//With 10 being the value of the matrix to traspose which isn't common, m the other uncommon value and n the common one
    for(i=0;i<m;i++){
        max=g(mult_trasp[i][j]);
        for(j=0;j<10;j++){
            mult_trasp[i][j]=g(mult_trasp[i][j]);
            if(mult_trasp[i][j]>max){
                max=mult_trasp[i][j];
                pos=j;
            }
        }
        tabMoy[i]=pos;
    }
    for(i=0;i<m;i++){
        valy=test_y[i];
    switch(valy){
    case 0: cont0++;
            if(tabMoy[i]==0){
            val0++;
            ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" Y"<<endl;
            }
            else{
                ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" N"<<endl;
            }
       break;
    case 1: cont1++;
            if(tabMoy[i]==1){
            val1++;
            ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" Y"<<endl;
            }
            else{
                ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" N"<<endl;
            }
        break;
    case 2: cont2++;
            if(tabMoy[i]==2){
            val2++;
            ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" Y"<<endl;
            }
            else{
                ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" N"<<endl;
            }
        break;
    case 3: cont3++;
            if(tabMoy[i]==3){
            val3++;
            ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" Y"<<endl;
            }
            else{
                ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" N"<<endl;
            }
        break;
    case 4: cont4++;
            if(tabMoy[i]==4){
            val4++;
            ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" Y"<<endl;
            }
            else{
                ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" N"<<endl;
            }
        break;
    case 5: cont5++;
            if(tabMoy[i]==5){
            val5++;
            ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" Y"<<endl;
            }
            else{
                ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" N"<<endl;
            }
        break;
    case 6: cont6++;
            if(tabMoy[i]==6){
            val6++;
            ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" Y"<<endl;
            }
            else{
                ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" N"<<endl;
            }
        break;
    case 7: cont7++;
            if(tabMoy[i]==7){
            val7++;
            ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" Y"<<endl;
            }
            else{
                ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" N"<<endl;
            }
        break;
    case 8: cont8++;
            if(tabMoy[i]==8){
            val8++;
            ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" Y"<<endl;
            }
            else{
                ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" N"<<endl;
            }
        break;
    case 9: cont9++;
            if(tabMoy[i]==9){
            val9++;
            ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" Y"<<endl;
            }
            else{
                ficheroSalida<<tabMoy[i]<<" "<<test_y[i]<<" N"<<endl;
            }
        break;
    }
    }
    val0=val0/cont0;
    val1=val1/cont1;
    val2=val2/cont2;
    val3=val3/cont3;
    val4=val4/cont4;
    val5=val5/cont5;
    val6=val6/cont6;
    val7=val7/cont7;
    val8=val8/cont8;
    val9=val9/cont9;
    ficheroSalida<<val0<<" "<<val1<<" "<<val2<<" "<<val3<<" "<<val4<<" "<<val5<<" "<<val6<<" "<<val7<<" "<<val8<<" "<<val9<<endl;

    }






















///////////////////////////////////////////////////////////
//           DO NOT MODIFY AFTER THIS LINE               //
///////////////////////////////////////////////////////////

void print(const FLOAT_TYPE *v, unsigned int n, string txt)
{
 cout << txt << "[" << n <<"]= { ";
 for(unsigned int i=0; i<n-1; ++i)
	 cout << v[i] <<", ";
 cout << v[n-1] << " }" << endl;
}

// Read the file \param filename and complete the matrix \param M of \param nb_lines by \param nb_cols elements with the content of the csv file. 
/*  The matrix \param M will be allocated by this function.
		\param filename : name of the csv file
		\param M : output matrix with content of the file
		\param nb_lines : number of lines reads 
		\param nb_cols  : number of entry per line (features)
*/
void loadCSV_to_matrix( char *filename, FLOAT_TYPE ***M,  unsigned int *nb_lines, unsigned int  *nb_cols)
{
    std::string delimiter = ","; // set the character used as separator
    ifstream file(filename);     // set filename to the stream
    if (file.is_open() != true ) // try to open the file
        {						 // if trouble return with error message
        cerr << " ** can't open file : " << filename << endl;
        return;
        }

    string line;				//else : start file the analysis
    std::vector< string > lines;  
	while(getline(file,line))   // First, Read all lines
		lines.push_back(line);  // and store them in "lines"
	file.close();				// close the file
	
	// Get size of data
    *nb_lines = lines.size();	// Second, determine the number of lines
    *nb_cols = 1 + std::count( lines[0].begin(), lines[0].end(), ','); // idem with the number of features

    // creating matrix M and filling it with csv values
    allocate( M, *nb_lines, *nb_cols );   // and allocate the memory space for M
#if defined(_OPENMP)
    #pragma omp parallel for                            // Concurrency (or parallel) for loop
#endif
    for(unsigned int i=0; i<*nb_lines; ++i) //Third, split each line
        {
        size_t pos = 0;
        std::string token;					
        unsigned int j = 0;
        while ((pos = lines[i].find(delimiter)) != std::string::npos) // search the separator token
            {
            token = lines[i].substr(0, pos); 				// extract the substring stopping at ','
            (*M)[i][j] = std::stof(token);  				// convert string to FLOAT_TYPE
            lines[i].erase(0, pos + delimiter.length()); 	// remove the analyzed number
            j++; 											// go to next entry in M
            }
        }
}

// Extract the features from the matrix \param M to \param X. X is a matrix of size nb_lines by nb_cols-1 (as the first column is the digit, ie the label)
void extract_features_from_CSV(FLOAT_TYPE **X, const FLOAT_TYPE * const* M, unsigned int nb_lines, unsigned int nb_cols)
{
    for(unsigned int i=0; i<nb_lines; ++i)
        for(unsigned int j=1; j<nb_cols; ++j)
            X[i][j-1] = M[i][j];

}

// Extract the labels from the matrix \param M to \param y. y is a vector of size nb_lines and corresponds to the first column of M.
void extract_labels_from_CSV(FLOAT_TYPE *y, const FLOAT_TYPE * const *M, unsigned int nb_lines)
{
     for(unsigned int i=0; i<nb_lines; ++i)
         y[i] = M[i][0];
}




