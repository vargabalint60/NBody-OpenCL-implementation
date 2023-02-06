__kernel void acc(
  __global double *X,
  __global double *Y,
  __global double *Z,
  __global double *VX,
  __global double *VY,
  __global double *VZ,
  __global double *AX,
  __global double *AY,
  __global double *AZ,
  __global const double *ABX,
  __global const double *ABY,
  __global const double *ABZ,
  __global const int *pairs,
  __global const double *M,
  __global const int *blockId) 
{
    int N = get_global_size(0);
    int i = get_global_id(0);
    AX[i] = 0;
    AY[i] = 0;
    AZ[i] = 0;
    int ind = 0;
    for (int j = 0; j < N - 1; j++) {
        ind = pairs[i*(N-1) + j];
        if (ind != -1) { 
            double r_x = X[i] - X[ind]; 
            double r_y = Y[i] - Y[ind];
            double r_z = Z[i] - Z[ind];
            double rSquared = r_x*r_x + r_y*r_y + r_z*r_z;
            double rCubed = rSquared * sqrt(rSquared);

            if (rSquared > 0.2) {
                AX[i] += - M[ind] * r_x / rCubed;   //missing G
                AY[i] += - M[ind] * r_y / rCubed;
                AZ[i] += - M[ind] * r_z / rCubed;
            }
            else {
                AX[i] += M[ind] * r_x / (5*rCubed); 
                AY[i] += M[ind] * r_y / (5*rCubed);
                AZ[i] += M[ind] * r_z / (5*rCubed);
            }
        }
        else {
            break;
        }    
    }

    AX[i] += ABX[blockId[i]];
    AY[i] += ABY[blockId[i]];
    AZ[i] += ABZ[blockId[i]];

    double dt = 0.005;
    VX[i] += AX[i]*dt;
    VY[i] += AY[i]*dt;
    VZ[i] += AZ[i]*dt;
    X[i] += VX[i]*dt;
    Y[i] += VY[i]*dt;
    Z[i] += VZ[i]*dt;
}

__kernel void Pairs(
  __global const double *X,
  __global const double *Y,
  __global const double *Z,
  __global int *pairs,
  __global const int *blockId) 
{
    int N = get_global_size(0);
    int i = get_global_id(0);
    int pos_f = 0;
    int pos_b = 0;
    for (int j = 0; j < N; j++) {
        if (j != i && blockId[i] == blockId[j]) {  //adds to pairs automatically
            pairs[i*(N-1) + pos_f] = j;            //if in the same block
            pos_f++;
        }
        else if (j != i && blockId[i] != blockId[j]) {   //if not, calculates the distance
            double r_x = X[i] - X[j]; 
            double r_y = Y[i] - Y[j];
            double r_z = Z[i] - Z[j];
            double rSquared = r_x*r_x + r_y*r_y + r_z*r_z;
            if (rSquared < 100) {
                pairs[i*(N-1) + pos_f] = j;
                pos_f++;
            }
            else {
                pairs[(i+1)*(N-1) - 1 - pos_b] = -1;   //fills up pairs with -1s from 
                pos_b++;                               //the back
            }                           


        }    
    }
}