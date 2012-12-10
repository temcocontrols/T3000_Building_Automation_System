#pragma once

#ifndef _FLEXSLIDEBAR_DEFINE_H_
#define _FLEXSLIDEBAR_DEFINE_H_


#ifdef _BUILD_FLEXSLIDEBAR_
#define FLEXSLIDEBAR_EXPORT	__declspec(dllexport)
#pragma message (">>>FlexSliderBar.dll ExPorted")
#else
#define FLEXSLIDEBAR_EXPORT	__declspec(dllimport)
#pragma message	("<<<FlexSliderBar.dll Imported")
#endif


#endif