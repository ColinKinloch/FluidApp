//A long list of OpenCL error codes

#pragma once

#undef __GET_DEVICE_INFO_ERR
#undef __GET_PLATFORM_INFO_ERR
#undef __GET_DEVICE_IDS_ERR
#undef __GET_CONTEXT_INFO_ERR
#undef __GET_EVENT_INFO_ERR
#undef __GET_EVENT_PROFILE_INFO_ERR
#undef __GET_MEM_OBJECT_INFO_ERR
#undef __GET_IMAGE_INFO_ERR
#undef __GET_SAMPLER_INFO_ERR
#undef __GET_KERNEL_INFO_ERR
#undef __GET_KERNEL_WORK_GROUP_INFO_ERR
#undef __GET_PROGRAM_INFO_ERR
#undef __GET_PROGRAM_BUILD_INFO_ERR
#undef __GET_COMMAND_QUEUE_INFO_ERR
#undef __CREATE_CONTEXT_FROM_TYPE_ERR
#undef __GET_SUPPORTED_IMAGE_FORMATS_ERR
#undef __CREATE_BUFFER_ERR
#undef __CREATE_SUBBUFFER_ERR
#undef __CREATE_GL_BUFFER_ERR
#undef __CREATE_IMAGE2D_ERR
#undef __CREATE_IMAGE3D_ERR
#undef __SET_MEM_OBJECT_DESTRUCTOR_CALLBACK_ERR
#undef __CREATE_USER_EVENT_ERR
#undef __SET_USER_EVENT_STATUS_ERR
#undef __SET_EVENT_CALLBACK_ERR
#undef __WAIT_FOR_EVENTS_ERR
#undef __CREATE_KERNEL_ERR
#undef __SET_KERNEL_ARGS_ERR
#undef __CREATE_PROGRAM_WITH_SOURCE_ERR
#undef __CREATE_PROGRAM_WITH_BINARY_ERR
#undef __BUILD_PROGRAM_ERR
#undef __CREATE_KERNELS_IN_PROGRAM_ERR
#undef __CREATE_COMMAND_QUEUE_ERR
#undef __SET_COMMAND_QUEUE_PROPERTY_ERR
#undef __ENQUEUE_READ_BUFFER_ERR
#undef __ENQUEUE_READ_BUFFER_RECT_ERR
#undef __ENQUEUE_WRITE_BUFFER_ERR
#undef __ENQUEUE_WRITE_BUFFER_RECT_ERR
#undef __ENQEUE_COPY_BUFFER_ERR
#undef __ENQEUE_COPY_BUFFER_RECT_ERR
#undef __ENQUEUE_READ_IMAGE_ERR
#undef __ENQUEUE_WRITE_IMAGE_ERR
#undef __ENQUEUE_COPY_IMAGE_ERR
#undef __ENQUEUE_COPY_IMAGE_TO_BUFFER_ERR
#undef __ENQUEUE_COPY_BUFFER_TO_IMAGE_ERR
#undef __ENQUEUE_MAP_BUFFER_ERR
#undef __ENQUEUE_MAP_IMAGE_ERR
#undef __ENQUEUE_UNMAP_MEM_OBJECT_ERR
#undef __ENQUEUE_NDRANGE_KERNEL_ERR
#undef __ENQUEUE_TASK_ERR
#undef __ENQUEUE_NATIVE_KERNEL
#undef __ENQUEUE_MARKER_ERR
#undef __ENQUEUE_WAIT_FOR_EVENTS_ERR
#undef __ENQUEUE_BARRIER_ERR
#undef __UNLOAD_COMPILER_ERR
#undef __FLUSH_ERR
#undef __FINISH_ERR

#undef __GET_PLATFORM_IDS_ERR
#undef __GET_GL_OBJECT_INFO_ERR
#undef __CREATE_SAMPLER_ERR
#undef __ENQUEUE_ACQUIRE_GL_ERR
#undef __ENQUEUE_RELEASE_GL_ERR

#define __CL_ENABLE_EXCEPTIONS
#define __CL_USER_OVERRIDE_ERROR_STRINGS
#define __GET_DEVICE_INFO_ERR "clGetDeviceInfo"
#define __GET_PLATFORM_INFO_ERR "clGetPlatformInfo"
#define __GET_DEVICE_IDS_ERR "clGetDeviceIds"
#define __GET_CONTEXT_INFO_ERR "clGetContextInfo"
#define __GET_EVENT_INFO_ERR "clGetEventInfo"
#define __GET_EVENT_PROFILE_INFO_ERR "clGetEventProfileInfo"
#define __GET_MEM_OBJECT_INFO_ERR "clGetMemObjectInfo"
#define __GET_IMAGE_INFO_ERR "clGetImageInfo"
#define __GET_SAMPLER_INFO_ERR "clGetSampleInfo"
#define __GET_KERNEL_INFO_ERR "clGetKernelInfo"
#define __GET_KERNEL_WORK_GROUP_INFO_ERR "clGetKernelWorkGroupInfo"
#define __GET_PROGRAM_INFO_ERR "clGetProgramInfo"
#define __GET_PROGRAM_BUILD_INFO_ERR "clGetProgramBuildInfo"
#define __GET_COMMAND_QUEUE_INFO_ERR "clGetCommandQueueInfo"
#define __CREATE_CONTEXT_FROM_TYPE_ERR "clCreateContxtFromType"
#define __GET_SUPPORTED_IMAGE_FORMATS_ERR "clGetSupportedImageFormats"
#define __CREATE_BUFFER_ERR "clCreateBuffer"
#define __CREATE_SUBBUFFER_ERR "clCreateSubBuffer"
#define __CREATE_GL_BUFFER_ERR "clCreateGLBuffer"
#define __CREATE_IMAGE2D_ERR "clCreateImage2D"
#define __CREATE_IMAGE3D_ERR "clCreateImage3D"
#define __SET_MEM_OBJECT_DESTRUCTOR_CALLBACK_ERR "clSetMemObjectDestructorCallback"
#define __CREATE_USER_EVENT_ERR "clCreateUserEvent"
#define __SET_USER_EVENT_STATUS_ERR "clSetUserEventStatus"
#define __SET_EVENT_CALLBACK_ERR "clSetEventCallback"
#define __WAIT_FOR_EVENTS_ERR "clWaitForEvents"
#define __CREATE_KERNEL_ERR "clCreateKernel"
#define __SET_KERNEL_ARGS_ERR "clSetKernelArgs"
#define __CREATE_PROGRAM_WITH_SOURCE_ERR "clCreateProgramWithSource"
#define __CREATE_PROGRAM_WITH_BINARY_ERR "clCreateProgramWithBinary"
#define __BUILD_PROGRAM_ERR "clBuildProgram"
#define __CREATE_KERNELS_IN_PROGRAM_ERR "clCreateKernelsInProgram"
#define __CREATE_COMMAND_QUEUE_ERR "clCreateCommandQueue"
#define __SET_COMMAND_QUEUE_PROPERTY_ERR "clSetCommandQueueProperty"
#define __ENQUEUE_READ_BUFFER_ERR "clEnqueueReadBuffer"
#define __ENQUEUE_READ_BUFFER_RECT_ERR "clEnqueueReadBufferRect"
#define __ENQUEUE_WRITE_BUFFER_ERR "clEnqueueWriteBuffer"
#define __ENQUEUE_WRITE_BUFFER_RECT_ERR "clEnqueueWriteBufferRect"
#define __ENQEUE_COPY_BUFFER_ERR "clEnqueueCopyBuffer"
#define __ENQEUE_COPY_BUFFER_RECT_ERR "clEnqueueCopyBufferRect"
#define __ENQUEUE_READ_IMAGE_ERR "clEnqueueReadImage"
#define __ENQUEUE_WRITE_IMAGE_ERR "clEnqueueWriteImage"
#define __ENQUEUE_COPY_IMAGE_ERR "clEnqueueCopyImage"
#define __ENQUEUE_COPY_IMAGE_TO_BUFFER_ERR "clEnqueueCopyImageToBuffer"
#define __ENQUEUE_COPY_BUFFER_TO_IMAGE_ERR "clEnqueueCopyBufferToImage"
#define __ENQUEUE_MAP_BUFFER_ERR "clEnqueueMapBuffer"
#define __ENQUEUE_MAP_IMAGE_ERR "clEnqueueMapImage"
#define __ENQUEUE_UNMAP_MEM_OBJECT_ERR "clEnqueueUnmapMemObject"
#define __ENQUEUE_NDRANGE_KERNEL_ERR "clEnqueueNDRangeKernel"
#define __ENQUEUE_TASK_ERR "clEnqueueTask"
#define __ENQUEUE_NATIVE_KERNEL "clEnqueueNativeKernel"
#define __ENQUEUE_MARKER_ERR "clEnqueueMarker"
#define __ENQUEUE_WAIT_FOR_EVENTS_ERR "clEnqueueWaitForEvents"
#define __ENQUEUE_BARRIER_ERR "clEnqueueBarriers"
#define __UNLOAD_COMPILER_ERR "clUnloadCompiler"
#define __FLUSH_ERR "clFlush"
#define __FINISH_ERR "clFinish"

#define __GET_PLATFORM_IDS_ERR "cklGetPlatformIds"
#define __GET_GL_OBJECT_INFO_ERR "cklGLObjectInfo"
#define __CREATE_SAMPLER_ERR "cklCreateSampler"
#define __ENQUEUE_ACQUIRE_GL_ERR "cklEnqueueAcuireGL"
#define __ENQUEUE_RELEASE_GL_ERR "cklReleaseGL"

