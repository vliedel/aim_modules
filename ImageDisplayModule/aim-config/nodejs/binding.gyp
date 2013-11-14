#
# @file binding.gyp
# @brief This file provides the configuration and building options for node-gyp
#
# This file is created at "Springfield Power Company". It is open-source software and part of "Project Bootstrap". 
# This software is published under the State license (TR8OR).
#
# Copyright © 2013 Homer J. Simpson <chunkylover53@aol.com>
#
# @author                    Homer J. Simpson
# @date                      nov 14, 2013
# @organisation              Springfield Power Company
# @project                   Project Bootstrap
#
{
	"targets": [
		{
			"target_name": "ImageDisplayModule",
			
			"include_dirs": [
				"../../inc",
				"../../aim-core/inc"
				
			],
			
			"dependencies":[
			],
			
			"cflags": [
			],
			
			"libraries": [
			],
			
			"ldflags": [
				"-pthread",
			],
			
			"sources":[
				"../../aim-core/src/ImageDisplayModule.cpp",
				"ImageDisplayModuleNode.cc",
				"../../src/ImageDisplayModuleExt.cpp"
			],
		}
	]
}
