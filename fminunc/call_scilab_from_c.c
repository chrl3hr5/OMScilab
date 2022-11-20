#include <stdio.h> /* stderr */
#include <stdlib.h> /* Provide functions to perform memory allocation */
#include "api_scilab.h" /* Provide functions to access to the memory of Scilab */
#include "call_scilab.h" /* Provide functions to call Scilab engine */

/* Function to append three strings */
char* string_append(char *first_string, char *second_string, char *third_string)
{
	int first_string_length = strlen(first_string);
	int second_string_length = strlen(second_string);
	int third_string_length = strlen(third_string);
	int size = first_string_length + second_string_length + third_string_length + 1;
	char *new_string = calloc(size, sizeof(char));
	
	for (int i = 0; i < first_string_length; i++)
		new_string[i] = first_string[i];

	for (int i = 0; i < second_string_length; i++)
		new_string[first_string_length + i] = second_string[i];
	
	for (int i = 0; i < third_string_length; i++)
		new_string[first_string_length + second_string_length + i] = third_string[i];

	new_string[size - 1] = '\0';

	return new_string;
}

/* Function to interact with Scilab */
double* Scilab_Operation(char *objective_function, char *starting_point, char *optimization_function, double* results)
{
	/****** Starting Scilab **********/
	StartScilab(getenv("SCI"), NULL, NULL);

	/****** Scilab Tasks *******/
	/* Creating objective function */
	char *input_objective_function = string_append("function f=objective_function(x); f = ",objective_function,"; endfunction");
	printf("%s\n",input_objective_function);
	SendScilabJob(input_objective_function);
	
	/* Setting parameter values for initial guess */
	char *input_starting_point = string_append("x0 = ", starting_point, ";");
	printf("%s\n",input_starting_point);
	SendScilabJob(input_starting_point);
	
	/* Performing optimization */
	char *input_optimization_function = string_append("[xopt,fopt] = ", optimization_function, "(objective_function,x0);");
	printf("%s\n",input_optimization_function);
	SendScilabJob(input_optimization_function);
    
	/****** Obtaining Results in C *******/
	/* Read the xopt matrix */
	int first_row = 0, first_col = 0;
	double *first_matrix_of_double = NULL;
	int first_i = 0, first_j = 0;
	char first_variable_to_be_retrieved[] = "xopt";

	/* First, retrieve the size of the matrix */
	SciErr first_sciErr;
	first_sciErr = readNamedMatrixOfDouble(getenv("SCI"), first_variable_to_be_retrieved, &first_row, &first_col, NULL);
	if(first_sciErr.iErr)
	{
		printError(&first_sciErr, 0);
	}

	/* Alloc the memory */
	first_matrix_of_double = (double*)malloc((first_row*first_col)*sizeof(double));

	/* Load the matrix */
	first_sciErr = readNamedMatrixOfDouble(getenv("SCI"), first_variable_to_be_retrieved, &first_row, &first_col, first_matrix_of_double);
	if(first_sciErr.iErr)
	{
		printError(&first_sciErr, 0);
	}
	 
	/* Read the fopt matrix */
	int second_row = 0, second_col = 0;
	double *second_matrix_of_double = NULL;
	int second_i = 0, second_j = 0;
	char second_variable_to_be_retrieved[] = "fopt";

	/* First, retrieve the size of the matrix */
	SciErr second_sciErr;
	second_sciErr = readNamedMatrixOfDouble(getenv("SCI"), second_variable_to_be_retrieved, &second_row, &second_col, NULL);
	if(second_sciErr.iErr)
	{
		printError(&second_sciErr, 0);
	}

	/* Alloc the memory */
	second_matrix_of_double = (double*)malloc((second_row*second_col)*sizeof(double));

	/* Load the matrix */
	second_sciErr = readNamedMatrixOfDouble(getenv("SCI"), second_variable_to_be_retrieved, &second_row, &second_col, second_matrix_of_double);
	if(second_sciErr.iErr)
	{
		printError(&second_sciErr, 0);
	}
	 
	/* Results */
	int i = 0;
	int results_i;
	int results_array_index = first_row*first_col + second_row*second_col;
	for(first_j = 0 ; first_j < first_row ; first_j++)
	 {
	  for(first_i = 0 ; first_i < first_col ; first_i++)
	   {
		results[i] = first_matrix_of_double[first_i * first_row + first_j];
		i = i + 1;
	   }
	 }
	for(second_j = 0 ; second_j < second_row ; second_j++)
	 {
	  for(second_i = 0 ; second_i < second_col ; second_i++)
	   {
		results[i] = second_matrix_of_double[second_i * second_row + second_j];
		i = i + 1;
	   }
	 }
	for(results_i = 0 ; results_i < results_array_index ; results_i++)
	 {
	  printf("\nResult %d: %5.2f\t",results_i + 1,results[results_i]);
	 }
	printf("\n");
	
	/****** Sending results to OpenModelica *******/
	return results;
}