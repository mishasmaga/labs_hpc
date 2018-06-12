#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <mpi.h>
#include <random>

using namespace std;

//! \param x
//! \return 4 / (1 - x * x)
double fh(double x) { return 4 / (1 + x * x); }

//! \param x
//! \return sqrt(x + sqrt(x))
double fi(double x) { return sqrt(x + sqrt(x)); }

int main(int argc, char *argv[]) {
  cout.precision(16);

  int N = 10000,  // n of local samples
      MASTER = 0, // standard id of master
      id,         // if of the current process
      processes;  // total number of processes

  // for some reason, initializing variables that are going to be used in MPI
  // Reduce throws a memcpy error, so the global sum variables must not be
  // initialized. however, this doesn't happen in all machines, so it's hard to
  // diagnose
  double sumf1 = 0, sumf2 = 0, // local sums for both functions
      sum_allf1, sum_allf2;    // global sum used in reduce

  // configure our PRNG for the Monte Carlo procedure
  using clock = chrono::high_resolution_clock;
  int seed = clock::now().time_since_epoch().count();
  mt19937_64 myMersenne = mt19937_64(seed);
  uniform_real_distribution<double> myUniformDistribution =
      uniform_real_distribution<double>(0, 1);

  // Initialize MPI
  MPI_Init(&argc, &argv);
  // Get the number of processes
  MPI_Comm_size(MPI_COMM_WORLD, &processes);
  // Determine the rank of this process
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  // start timer
  double start = MPI_Wtime();

  // expected value of fh = M_PI
  // expected value of fi = 1.04530130813919
  if (id != MASTER) {
    // the slave processes compute N samples from the function to be integrated
    for (int i = 0; i < N; i ++) {
      sumf1 += fh(myUniformDistribution(myMersenne));
      sumf2 += fi(myUniformDistribution(myMersenne));
    }
    // each slave takes the average of the N results they sampled
    sumf1 /= N;
    sumf2 /= N;
  }

  // reduce all estimates to a single sum in the master node
  MPI_Reduce(&sumf1, &sum_allf1, 1, MPI_DOUBLE, MPI_SUM, MASTER,
             MPI_COMM_WORLD);
  MPI_Reduce(&sumf2, &sum_allf2, 1, MPI_DOUBLE, MPI_SUM, MASTER,
             MPI_COMM_WORLD);

  // master prints average of estimates
  if (id == MASTER) {
      // -1 in the following lines because master does not participate in the sampling
    cout << "       F1: " << (sum_allf1 / (double) (processes - 1)) << endl;
    cout << "       F2: " << (sum_allf2 / (double) (processes - 1)) << endl;
    cout << "TIME (S): " << MPI_Wtime() - start << endl;
  }

  //  Terminate MPI.
  MPI_Finalize();
  return 0;
}