__kernel void vector_add(
  __global const double *A,
  __global const double *B,
  __global double *C)
{ 
    // Get the index of the current element to be processed
    int i = get_global_id(0);
    C[i]=A[i]+B[i];
    
}

__kernel void acc(
  __global const double *X,
  __global const double *Y,
  __global const double *Z,
  __global double *AX,
  __global double *AY,
  __global double *AZ,
  __global const double *M) 
{
    int N = get_global_size(0);
    int i = get_global_id(0);
    AX[i] = 0;
    AY[i] = 0;
    AZ[i] = 0;
    for (int j = 0; j < N; j++) {
        if (j != i) { 
            double r_x = X[i] - X[j]; 
            double r_y = Y[i] - Y[j];
            double r_z = Z[i] - Z[j];
            double rSquared = r_x*r_x + r_y*r_y + r_z*r_z;
            double rCubed = rSquared * sqrt(rSquared);

            if (rSquared > 3) {
                AX[i] += - M[j] * r_x / rCubed;   //missing G
                AY[i] += - M[j] * r_y / rCubed;
                AZ[i] += - M[j] * r_z / rCubed;
            }
            else {
                AX[i] += M[j] * r_x / (3*rCubed);   //asdasdasd
                AY[i] += M[j] * r_y / (3*rCubed);
                AZ[i] += M[j] * r_z / (3*rCubed);
            }
        }    
    }
}

__kernel void move(
  __global double *X,
  __global double *Y,
  __global double *Z,
  __global double *VX,
  __global double *VY,
  __global double *VZ,
  __global const double *AX,
  __global const double *AY,
  __global const double *AZ)
{
    int i = get_global_id(0);
    double dt = 0.005;
    VX[i] += AX[i]*dt;
    VY[i] += AY[i]*dt;
    VZ[i] += AZ[i]*dt;
    X[i] += VX[i]*dt;
    Y[i] += VY[i]*dt;
    Z[i] += VZ[i]*dt;
}
