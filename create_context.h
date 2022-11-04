#pragma once

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
#include "point.h"


using namespace cl;
Program program;
Context context;
CommandQueue queue;

void create_context() {
	try {
		vector<Platform> platforms;
		Platform::get(&platforms);

		vector<Device> devices;

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
		
		queue = CommandQueue(context, devices[0]);

		// Read source file
		std::ifstream sourceFile("euler_cromer.cl");
		std::string sourceCode(
			std::istreambuf_iterator<char>(sourceFile),
			(std::istreambuf_iterator<char>()));
		Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));

		// Make program of the source code in the context
		program = Program(context, source);

		// Build program for these specific devices
		try {
      		program.build(devices);
    	}
    	catch (Error error) {
      		oclPrintError(error);
		    // Detailed build errors:
		    std::cerr << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(devices[0]) << std::endl;
		    std::cerr << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(devices[0]) << std::endl;
		    std::cerr << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << std::endl;
		    throw error;
   		}

	} catch(Error error) {
		oclPrintError(error);
	}
}

void calcAcc(Point points, int N) {
    Kernel kernel(program, "acc");

    Buffer buffer_X = Buffer(context, CL_MEM_READ_ONLY, sizeof(double) * N);
    Buffer buffer_Y = Buffer(context, CL_MEM_READ_ONLY, sizeof(double) * N);
    Buffer buffer_Z = Buffer(context, CL_MEM_READ_ONLY, sizeof(double) * N);
    Buffer buffer_AX = Buffer(context, CL_MEM_WRITE_ONLY, sizeof(double) * N);
    Buffer buffer_AY = Buffer(context, CL_MEM_WRITE_ONLY, sizeof(double) * N);
    Buffer buffer_AZ = Buffer(context, CL_MEM_WRITE_ONLY, sizeof(double) * N);
    Buffer buffer_M = Buffer(context, CL_MEM_READ_ONLY, sizeof(double) * N);

    queue.enqueueWriteBuffer(buffer_X, CL_TRUE, 0, sizeof(double) * N, points.x);
    queue.enqueueWriteBuffer(buffer_Y, CL_TRUE, 0, sizeof(double) * N, points.y);
    queue.enqueueWriteBuffer(buffer_Z, CL_TRUE, 0, sizeof(double) * N, points.z);
    queue.enqueueWriteBuffer(buffer_AX, CL_TRUE, 0, sizeof(double) * N, points.ax);
    queue.enqueueWriteBuffer(buffer_AY, CL_TRUE, 0, sizeof(double) * N, points.ay);
    queue.enqueueWriteBuffer(buffer_AZ, CL_TRUE, 0, sizeof(double) * N, points.az);
    queue.enqueueWriteBuffer(buffer_M, CL_TRUE, 0, sizeof(double) * N, points.m);

    kernel.setArg(0, buffer_X);
    kernel.setArg(1, buffer_Y);
    kernel.setArg(2, buffer_Z);
    kernel.setArg(3, buffer_AX);
    kernel.setArg(4, buffer_AY);
    kernel.setArg(5, buffer_AZ);
    kernel.setArg(6, buffer_M);

    NDRange _global_(N);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, _global_, cl::NullRange);

    queue.enqueueReadBuffer(buffer_AX, CL_TRUE, 0, sizeof(double) * N, points.ax);
    queue.enqueueReadBuffer(buffer_AY, CL_TRUE, 0, sizeof(double) * N, points.ay);
    queue.enqueueReadBuffer(buffer_AZ, CL_TRUE, 0, sizeof(double) * N, points.az);
}


void calcVelPos(Point points, int N) {
    Kernel kernel(program, "move");

    Buffer buffer_X = Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * N);
    Buffer buffer_Y = Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * N);
    Buffer buffer_Z = Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * N);
    Buffer buffer_VX = Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * N);
    Buffer buffer_VY = Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * N);
    Buffer buffer_VZ = Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * N);
    Buffer buffer_AX = Buffer(context, CL_MEM_READ_ONLY, sizeof(double) * N);
    Buffer buffer_AY = Buffer(context, CL_MEM_READ_ONLY, sizeof(double) * N);
    Buffer buffer_AZ = Buffer(context, CL_MEM_READ_ONLY, sizeof(double) * N);

    queue.enqueueWriteBuffer(buffer_X, CL_TRUE, 0, sizeof(double) * N, points.x);
    queue.enqueueWriteBuffer(buffer_Y, CL_TRUE, 0, sizeof(double) * N, points.y);
    queue.enqueueWriteBuffer(buffer_Z, CL_TRUE, 0, sizeof(double) * N, points.z);
    queue.enqueueWriteBuffer(buffer_VX, CL_TRUE, 0, sizeof(double) * N, points.vx);
    queue.enqueueWriteBuffer(buffer_VY, CL_TRUE, 0, sizeof(double) * N, points.vy);
    queue.enqueueWriteBuffer(buffer_VZ, CL_TRUE, 0, sizeof(double) * N, points.vz);
    queue.enqueueWriteBuffer(buffer_AX, CL_TRUE, 0, sizeof(double) * N, points.ax);
    queue.enqueueWriteBuffer(buffer_AY, CL_TRUE, 0, sizeof(double) * N, points.ay);
    queue.enqueueWriteBuffer(buffer_AZ, CL_TRUE, 0, sizeof(double) * N, points.az);    

    kernel.setArg(0, buffer_X);
    kernel.setArg(1, buffer_Y);
    kernel.setArg(2, buffer_Z);
    kernel.setArg(3, buffer_VX);
    kernel.setArg(4, buffer_VY);
    kernel.setArg(5, buffer_VZ);
    kernel.setArg(6, buffer_AX);
    kernel.setArg(7, buffer_AY);
    kernel.setArg(8, buffer_AZ);

    NDRange _global_(N);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, _global_, cl::NullRange);

    queue.enqueueReadBuffer(buffer_X, CL_TRUE, 0, sizeof(double) * N, points.x);
    queue.enqueueReadBuffer(buffer_Y, CL_TRUE, 0, sizeof(double) * N, points.y);
    queue.enqueueReadBuffer(buffer_Z, CL_TRUE, 0, sizeof(double) * N, points.z);
    queue.enqueueReadBuffer(buffer_VX, CL_TRUE, 0, sizeof(double) * N, points.vx);
    queue.enqueueReadBuffer(buffer_VY, CL_TRUE, 0, sizeof(double) * N, points.vy);
    queue.enqueueReadBuffer(buffer_VZ, CL_TRUE, 0, sizeof(double) * N, points.vz);
}