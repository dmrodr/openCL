#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define ITERACOES 8000
#define THREADS 8

const char* kernel_source = 
"__kernel void incrementa(__global int* acumulador, int iteracoes) {    \n"
"   int id = get_global_id(0);                                           \n"
"   for (int i = 0; i < iteracoes; i++) {                                \n"
"       atomic_add(&acumulador[0], 1);                                   \n"
"   }                                                                     \n"
"}";

int main() {
    int acumulador_global = 0;
    int iteracoes = ITERACOES;
    int threads = THREADS;

    // Plataforma e dispositivo OpenCL
    cl_platform_id platform;
    clGetPlatformIDs(1, &platform, NULL);

    cl_device_id device;
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Contexto e fila de comandos
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

    // Buffer para o acumulador global
    cl_mem acumulador_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, NULL);

    // Inicializa o buffer
    clEnqueueWriteBuffer(queue, acumulador_buffer, CL_TRUE, 0, sizeof(int), &acumulador_global, 0, NULL, NULL);

    // Compilação do kernel
    cl_program program = clCreateProgramWithSource(context, 1, &kernel_source, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "incrementa", NULL);

    // Define os argumentos do kernel
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &acumulador_buffer);
    clSetKernelArg(kernel, 1, sizeof(int), &iteracoes);

    // Configura a execução do kernel
    size_t global_work_size[1] = {threads}; // Número total de threads
    size_t local_work_size[1] = {1};         // Número de threads por grupo de trabalho

    // Executa o kernel
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
    clFinish(queue);

    // Lê o resultado
    clEnqueueReadBuffer(queue, acumulador_buffer, CL_TRUE, 0, sizeof(int), &acumulador_global, 0, NULL, NULL);

    // Exibe os resultados
    printf("Valor esperado do incrementador: %d\n", iteracoes * threads);
    printf("Valor real do incrementador: %d\n", acumulador_global);

    // Libera recursos
    clReleaseMemObject(acumulador_buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
