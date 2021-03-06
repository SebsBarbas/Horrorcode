// Clanu 2017-2018 - INSA Lyon - GE

#include <iostream>
//#if defined(_OPENMP)
    #include <omp.h>
//#endif

#define FLOAT_TYPE float

#include "common_functions.h"
#include "clanu_functions.h"
#include "timing_functions.h"



using namespace std;

int main(int argc, char *argv[])
{
    // Test some compialtion options
    #if defined(_OPENMP)
        cout << " OPENMP is activated  : great! " << endl;
    #else
        cout << " OPENMP is not activated  (good for debug)" << endl;
    #endif

    #ifdef __FAST_MATH__
        cout << " fast-math is activated : great! " << endl;
    #else
        cout << " fast-math is strangely not activated " << endl;
    #endif


        ///////////////////////////////////////////////////////////
        //           START YOUR MODIFICATIONS HERE               //
        ///////////////////////////////////////////////////////////

        FLOAT_TYPE tau;
        unsigned int max_it;

        if( argc < 5)
            {
                cerr << " Usage : " << argv[0] << " Train_filename.csv Test_filename.csv max_it tau" << endl;
                return -1;
            }

        max_it = stoul( argv[3] );
        tau    = stof ( argv[4] );
        // Summarizing options
        cout << " ** summarize options : " << endl;
        cout << " \t Training file : " << argv[1] << endl;
        cout << " \t Testing  file : " << argv[2] << endl;
        cout << " \t max_it = " << max_it << endl;
        cout << " \t tau    = " << tau    << endl;

float accuracy_max;
float **Teta_max=nullptr;
float current_ac;

        cout << "Reading and initializing ... This may take a while (~20-30s) " << endl;
        tic();

        // read TRAINING CSV file
        FLOAT_TYPE **CSV=nullptr;
        unsigned int CSV_m, CSV_n;
        loadCSV_to_matrix( argv[1], &CSV,  &CSV_m, &CSV_n);

        // Extract features X and labels y
        unsigned int m = CSV_m;
        unsigned int n = CSV_n - 1; // the first column contains the labels
        FLOAT_TYPE **X = nullptr; allocate( &X, m, n);
        FLOAT_TYPE  *y = nullptr; allocate( &y, m);

        extract_features_from_CSV( X, CSV, CSV_m, CSV_n );
        extract_labels_from_CSV  ( y, CSV, CSV_m );
        destroy( &CSV, CSV_m);

        // Read TESTING CSV file
        loadCSV_to_matrix( argv[2], &CSV,  &CSV_m, &CSV_n);

        // Extract features test_X and labels test_y
        unsigned int test_m = CSV_m;
        FLOAT_TYPE **test_X = nullptr; allocate( &test_X, m, n);
        FLOAT_TYPE  *test_y = nullptr; allocate( &test_y, m);

        extract_features_from_CSV( test_X, CSV, test_m, CSV_n );
        extract_labels_from_CSV  ( test_y, CSV, test_m );
        destroy( &CSV, CSV_m);


        // Allocate Theta variable
        FLOAT_TYPE **Theta=nullptr; allocate(&Theta, 10, n);

        tac();
        cout << "Reading and initialization time : " << duration() << "s " << endl;

        // Training
        FLOAT_TYPE cumulative_error;
        zeros(Theta, 10, n);
        FLOAT_TYPE *d_c_k_ant=nullptr; allocate(&d_c_k_ant,n);
        FLOAT_TYPE *d_c_k_helper=nullptr; allocate(&d_c_k_helper,n);
        zeros(d_c_k_ant,10);
        float Bk;
        FLOAT_TYPE *r=nullptr; allocate(&r,n);
         zeros(r,n);
        for(unsigned int k=0; k < max_it; k++)
            {
            cumulative_error = 0;
            zeros(d_c_k_ant,n);
            tic();
        #if defined(_OPENMP)
            #pragma omp parallel for reduction(+:cumulative_error)  // Concurrency (or parallel) for loop
        #endif
            for(unsigned int c=0; c<10; c++)
                {
               FLOAT_TYPE *d_c_k=nullptr; allocate(&d_c_k,n);       // ( definied and allocated here for concurrency )
               zeros( d_c_k, n);
               FLOAT_TYPE *theta_c_k = Theta[c];                    //  linked on Theta, for easier reading


                // d_c_k = {0,0,0...} i.e. initialization
                for(unsigned int i=0; i<m; i++)
                    {
                    FLOAT_TYPE y_c_i = (y[i]==c)?1.0:0.0;            // y_c_i = 1 if y[i] == c
                                                                //         0  otherwise

                    FLOAT_TYPE h_theta_c_i =
                            g( dot_product( theta_c_k, X[i], n ) ) - y_c_i; // h_theta_c_i = g( theta_c_k . X[i] ) - y_c_i

                    mac_v_v_s( d_c_k, X[i], h_theta_c_i, n );    //  d_c_k += X[i] * h_theta_c_i

                    cumulative_error += abs(h_theta_c_i);            // ( used for evolution tracking )
                    }
                //Empezar a modificar aquí
                if(c==0){
                    for(unsigned int l=0;l<n;l++){
                        d_c_k_ant[l]=d_c_k[l];
                    }
                    mul_v_s( d_c_k, d_c_k, -tau / m,     n);        //  d_c_k *= - tau / m
                    sum_2v( theta_c_k, theta_c_k, d_c_k, n);        //  theta_c_k+1 = theta_c_k + d_c_k
                    destroy(&d_c_k);                                // ( free d_c_k )
                }
                else{
                for(unsigned int l=0; l<n; l++){
                   d_c_k_helper[l]=d_c_k[l];
                }
                //copy(d_c_k_helper,d_c_k,n);
                for(unsigned int l=0;l<n;l++)
                    r[l]=d_c_k[l]-d_c_k_ant[l];
                //sub_2v(r,d_c_k,d_c_k_ant,n);
                FLOAT_TYPE *vzero=nullptr;allocate(&vzero,n);
                zeros(vzero,n);
                Bk=dot_product(d_c_k,r,n)/norm_2v_sqr(d_c_k_ant,vzero,n);
                mul_v_s(d_c_k_ant,d_c_k_ant,Bk,n);
                sum_2v(d_c_k,d_c_k,d_c_k_ant,n);
                //Aquí ya es como antes
                 mul_v_s( d_c_k, d_c_k, -tau / m,     n);        //  d_c_k *= - tau / m
                sum_2v( theta_c_k, theta_c_k, d_c_k, n);        //  theta_c_k+1 = theta_c_k + d_c_k
                destroy(&d_c_k);
                destroy(&vzero);
                for(unsigned int l=0; l<n; l++){
                  d_c_k_ant[l]=d_c_k_helper[l];
                }
                }
                //copy(d_c_k_ant,d_c_k_helper,n);
                // ( free d_c_k )
                }
            tac();
            cout << "it : " << k << "\t time : " << duration() << " s\t error : "<< cumulative_error/(10*m)<<endl;

        current_ac=Accuracy(Theta,test_X,test_y,m,n);
            if(accuracy_max<current_ac){
                Teta_max=Theta;
                accuracy_max=current_ac;
            }
           if(k%5==0){
                cout<<endl;
                cout<<" Valeur du accuracy dans "<<k<<" ="<<current_ac<<endl;
            }
            cout << endl;
        }

        cout<<" Valeur du accuracy dans 59="<<current_ac<<endl;
        //save_teta(Theta);
        save_teta("data/TetaC.csv",Teta_max, 10, n);
        //open_teta("data/Teta.txt",Theta, 10, n);

        // Test with data at "test_index" from test.csv
        unsigned int test_index=18;
        FLOAT_TYPE *prob=nullptr; allocate(&prob, 10);
        FLOAT_TYPE max_prob;
        unsigned int c_prob = 0;
        prob[0] = g( dot_product( Theta[0], test_X[test_index], n ) );
        max_prob = prob[0];
        for(unsigned int c=1; c<10; c++)
            {
            prob[c] = g( dot_product( Theta[c], test_X[test_index], n ) );
            if( max_prob < prob[c])
                {
                max_prob = prob[c];
                c_prob   = c;
                }
            }
        cout << " The value at " << test_index << " should be : " << test_y[test_index] << " and the prediction done give : "  << c_prob;
        if( test_y[test_index] == c_prob ) cout << "  Good prediction :) !!" << endl;
        else cout << " Prediction error :( !!" << endl;
        print(prob, 10, " probabilities : ");




        // free memory
        destroy( &prob   );
        destroy( &y      );
        destroy( &test_y );

        destroy( &Theta, 10);
        destroy( &X, m     );
        destroy( &test_X, test_m );















cout << " end." << endl;
return 0;
}
