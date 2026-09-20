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

    long long prefix_sum = 0;

    MPI_Scan(
        &local_sum,
        &prefix_sum,
        1,
        MPI_LONG_LONG,
        MPI_SUM,
        MPI_COMM_WORLD
    );

    long long sum_before_me =
        prefix_sum - local_sum;

    long long K =
        (long long)(rank + 1) * chunk_size;

    long long expected_prefix =
        K * (K + 1) / 2;

    printf(
        "Rank %d: local_sum = %lld, prefix_sum = %lld, sum_before_me = %lld, expected_prefix = %lld\n",
        rank,
        local_sum,
        prefix_sum,
        sum_before_me,
        expected_prefix
    );

    if (rank == size - 1) {

        long long expected =
            (long long)N * (N + 1) / 2;

        printf(
            "\n[Scan] Final prefix = %lld\n",
            prefix_sum
        );

        printf(
            "[Scan] Expected     = %lld\n",
            expected
        );

        printf(
            "[Scan] Correct?     = %s\n",
            prefix_sum == expected ? "YES" : "NO"
        );
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {

        double elapsed =
            MPI_Wtime() - start;

        printf(
            "[Scan] Time = %.6f sec\n",
            elapsed
        );
    }

    free(array);
    free(local_chunk);

    MPI_Finalize();

    return 0;
}
