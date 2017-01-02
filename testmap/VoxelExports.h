/*
 * VoxelExports.h
 *
 *  Created on: 2016. 11. 23.
 *      Author: josco
 */

#ifndef VOXEL_EXPORT_H
#define VOXEL_EXPORT_H

#ifdef VOXEL_STATIC_DEFINE
#  define VOXEL_EXPORT
#  define VOXEL_NO_EXPORT
#else
#  ifndef VOXEL_EXPORT
#    ifdef voxel_EXPORTS
        /* We are building this library */
#      define VOXEL_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VOXEL_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VOXEL_NO_EXPORT
#    define VOXEL_NO_EXPORT
#  endif
#endif

#ifndef VOXEL_DEPRECATED
#  define VOXEL_DEPRECATED __declspec(deprecated)
#endif

#ifndef VOXEL_DEPRECATED_EXPORT
#  define VOXEL_DEPRECATED_EXPORT VOXEL_EXPORT VOXEL_DEPRECATED
#endif

#ifndef VOXEL_DEPRECATED_NO_EXPORT
#  define VOXEL_DEPRECATED_NO_EXPORT VOXEL_NO_EXPORT VOXEL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VOXEL_NO_DEPRECATED
#    define VOXEL_NO_DEPRECATED
#  endif
#endif

#endif
