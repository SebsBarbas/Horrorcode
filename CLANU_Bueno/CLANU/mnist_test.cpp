// Clanu 2017-2018 - INSA Lyon - GE

#include <iostream>
#include <cmath>
#define FLOAT_TYPE float

#include "common_functions.h"
#include "clanu_functions.h"
#include "timing_functions.h"

using namespace std;

int main(int argc, char *argv[])
{
    int m,n;
    FLOAT_TYPE **Theta=nullptr;

if( argc < 3)
    {
        cerr << " Usage : " << argv[0] << "Theta_filename Test_filename.csv" << endl;
        return -1;
    }

// Summarizing options
cout << " ** summarize options : " << endl;
cout << " \t data/Teta.txt : " << argv[1] << endl;
cout << " \t Testing  file : " << argv[2] << endl; //

///////////////////////////////////////////////////////////
//           START YOUR MODIFICATIONS HERE               //
///////////////////////////////////////////////////////////



FLOAT_TYPE **testx=nullptr;
unsigned int testx_m, testx_n;
// Read Theta_test CSV file     aqui deberiamos tener n=785 y m la que sea que es con la que vamos a trabajar todo el rato
loadCSV_to_matrix( argv[2], &testx,  &testx_m, &testx_n);
n=testx_n-1;
m=testx_m;
// read Theta buena la que calculamos en el training
allocate(&Theta,10,n);
open_teta("data/Theta.csv",Theta, 10, n);
// Extract features test_X and labels test_y
unsigned int test_m = testx_m;
FLOAT_TYPE **test_X = nullptr; allocate( &test_X, m, n);
FLOAT_TYPE  *test_y = nullptr; allocate( &test_y, m);

extract_features_from_CSV( test_X, testx, test_m, testx_n );
extract_labels_from_CSV  ( test_y, testx, test_m );
destroy( &testx, testx_m);

test_ac(Theta, test_X, test_y, m, n, "data/result.csv");






cout << " end." << endl;
return 0;
}
