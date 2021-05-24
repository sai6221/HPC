#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <malloc.h>
#include <time.h>
typedef struct
{
  int x;
} __attribute__ ((aligned (64))) elem;
int MKnapsack (int, int);
int max (int, int);
int n;
int *weights;
int *values;
int W;
elem **F;

int max (int a, int b)
{
  return (a > b ? a : b);
}


int MKnapsack (int i, int j)
{
  int value;
  if (((*(F + i) + j)->x) < 0)
    {
      if (j < weights[i])
    value = MKnapsack (i - 1, j);
      else
    {
      int a;
      int b;
#pragma omp parallel sections
      {
#pragma omp section
        {
          a = MKnapsack (i - 1, j);
        }
#pragma omp section
        {
          b = (values[i] + MKnapsack (i - 1, j - weights[i]));
        }
      }
      value = max (a, b);
    }
      (*(F + i) + j)->x = value;
    }
  return (*(F + i) + j)->x;
}


int main ()
{
  clock_t start, end;
  printf ("Enter number of items :");
  scanf ("%d", &n);
  printf ("--------------------------------------------------");
  weights = (int *) malloc ((n + 1) * sizeof (int));
  values = (int *) malloc ((n + 1) * sizeof (int));
  int i, j;
  printf ("\n");
  for (i = 1; i <= n; i++)
    {
      printf ("Enter weights one by one ");
      scanf ("%d", &weights[i]);
    }
  printf ("---------------------------------------------------");
  printf ("\n");
  for (i = 1; i <= n; i++)
    {
      printf ("Enter values one by one ");
      scanf ("%d", &values[i]);
    }
  printf ("---------------------------------------------------");
  printf ("\nEnter the Weight of the Knapsack : ");
  scanf ("%d", &W);
  F = (elem **) malloc ((n + 1) * sizeof (elem *));
  for (i = 0; i <= n; i++)
    F[i] = (elem *) malloc ((W + 1) * sizeof (elem));
  for (i = 0; i < (W + 1); i++)
    (*(F) + i)->x = 0;
  for (i = 0; i < (n + 1); i++)
    (*(F + i))->x = 0;
  for (i = 1; i <= n; i++)
    {
      for (j = 1; j <= W; j++)
    (*(F + i) + j)->x = -1;
    }
  start = clock ();
  int res;
#pragma omp parallel
  {
#pragma omp single nowait
    {
      res = MKnapsack (n, W);
    }
  }
  end = clock ();
  double time_taken = (double) (end - start) / (double) (CLOCKS_PER_SEC);
  printf ("Total marks of the student= %d\n", res);
  printf ("Time taken = %1f s", (double) (time_taken));
  free (weights);
  free (values);
  return 0;
}
