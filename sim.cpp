#define __NO_STD_VECTOR
#define __CL_ENABLE_EXCEPTIONS

#include <GL/glut.h>
#include <CL/cl.hpp>
#include <iostream>
#include <math.h>
#include <utility>
#include <fstream>
#include <string>
#include <oclutils.hpp>


using namespace cl;

int main() {
	// Create the two input vectors
	const int LIST_SIZE = 2000;
	double *A_h = new double[LIST_SIZE];
	double *B_h = new double[LIST_SIZE];
	for(int i = 0; i < LIST_SIZE; i++) {
		A_h[i] = (double)i/1000;
		B_h[i] = (double)(LIST_SIZE - i)/1000;
	}
	try {
		vector<Platform> platforms;
		Platform::get(&platforms);

		vector<Device> devices;
		Context context;

		for (auto p : platforms) {

			try {
				std::cout << p.getInfo<CL_PLATFORM_NAME>() << std::endl;
				std::cout << p.getInfo<CL_PLATFORM_VERSION>() << std::endl;
			
				// Select the default platform and create a context using this platform and the GPU
				cl_context_properties cps[3] = {
					CL_CONTEXT_PLATFORM,
					(cl_context_properties)(p)(),
					0
				};

				context = Context(CL_DEVICE_TYPE_GPU, cps);

				// Get a list of devices on this platform
				devices = context.getInfo<CL_CONTEXT_DEVICES>();

			} catch (Error error) {
				oclPrintError(error);
				continue;
			}

			if (devices.size() > 0)
				break;
		}

		if(devices.size() == 0) {
			throw Error(CL_INVALID_CONTEXT, "Failed to create a valid context!");
		}
		
		CommandQueue queue = CommandQueue(context, devices[0]);

		// Read source file
		std::ifstream sourceFile("vector_add_kernel.cl");
		std::string sourceCode(
			std::istreambuf_iterator<char>(sourceFile),
			(std::istreambuf_iterator<char>()));
		Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));

		// Make program of the source code in the context
		Program program = Program(context, source);

		// Build program for these specific devices
		program.build(devices);

		// Make kernel
		Kernel kernel(program, "vector_add");

		Buffer buffer_A = Buffer(context, CL_MEM_READ_ONLY, sizeof(double) * LIST_SIZE);
		Buffer buffer_B = Buffer(context, CL_MEM_READ_ONLY, sizeof(double) * LIST_SIZE);
		Buffer buffer_C = Buffer(context, CL_MEM_WRITE_ONLY, sizeof(double) * LIST_SIZE);

		queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(double) * LIST_SIZE, A_h);
		queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(double) * LIST_SIZE, B_h);

		kernel.setArg(0, buffer_A);
		kernel.setArg(1, buffer_B);
		kernel.setArg(2, buffer_C);

		NDRange _global_(LIST_SIZE);
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, _global_, cl::NullRange);

		double *C_h = new double[LIST_SIZE];
		queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(double) * LIST_SIZE, C_h);
		for(int i = 0; i < LIST_SIZE; i ++)
			std::cout << A_h[i] << " + " << B_h[i] << " = " << C_h[i] << std::endl;


	} catch(Error error) {
		oclPrintError(error);
	}
	return 1;
}