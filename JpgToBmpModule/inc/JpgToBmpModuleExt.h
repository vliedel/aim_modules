/**
 * @file JpgToBmpModuleExt.h
 * @brief JpgToBmpModule extension
 *
 * This file is created at "DoBots". It is open-source software and part of "AI". 
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2013 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     nov 20, 2013
 * @organisation             DoBots
 * @project                  AI
 */

#include <JpgToBmpModule.h>
#include <exif.h>
#include <AimSerializationHeader.hpp>

namespace rur {

/**
 * Your Description of this module.
 */
class JpgToBmpModuleExt: public JpgToBmpModule {
public:
	//! The constructor
	JpgToBmpModuleExt();

	//! The destructor
	virtual ~JpgToBmpModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the JpgToBmpModuleMain will stop the module
	bool Stop();

private:
	unsigned int mScaleNum;
	unsigned int mScaleDenom;
	AimSerializationHeader mSerialization;

	int getRotatedIdx(int rotation, int row, int col, int width, int height);

	void adjustRotatedSize(int rotation, int width, int height, int* newWidth, int* newHeight);

	void rotate(long_seq img, int width, int height, EXIFInfo exif, long_seq* newImg);

};

}

