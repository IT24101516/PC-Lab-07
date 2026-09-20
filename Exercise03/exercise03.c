#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000

int main(int argc, char **argv) {

    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0)
            printf("Number of processes must divide %d evenly.\n", N);

        MPI_Finalize();
        return 1;
    }

    int chunk_size = N / size;

    int *array = NULL;

    if (rank == 0) {

        array = malloc(N * sizeof(int));

        for (int i = 0; i < N; i++)
            array[i] = i + 1;
    }

    int *local_chunk =
        malloc(chunk_size * sizeof(int));

    double start = MPI_Wtime();

    MPI_Scatter(
        array,
        chunk_size,
        MPI_INT,
        local_chunk,
        chunk_size,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    long long local_sum = 0;

    for (int i = 0; i < chunk_size; i++)
        local_sum += local_chunk[i];

    printf(
        "Rank %d: local_sum = %lld\n",
        rank,
        local_sum
    );

    long long *all_sums = NULL;

    if (rank == 0)
        all_sums = malloc(size * sizeof(long long));

    MPI_Gather(
        &local_sum,
        1,
        MPI_LONG_LONG,
        all_sums,
        1,
        MPI_LONG_LONG,
        0,
        MPI_COMM_WORLD
    );

    if (rank == 0) {

        long long total_sum = 0;

        for (int i = 0; i < size; i++)
            total_sum += all_sums[i];

        double elapsed = MPI_Wtime() - start;

        long long expected =
            (long long)N * (N + 1) / 2;

        printf("\n[Gather] Total sum = %lld\n", total_sum);
        printf("[Gather] Expected  = %lld\n", expected);
        printf(
            "[Gather] Correct?   = %s\n",
            total_sum == expected ? "YES" : "NO"
        );
        printf("[Gather] Time      = %.6f sec\n", elapsed);
    }

    free(array);
    free(local_chunk);
    free(all_sums);

    MPI_Finalize();

    return 0;
}
