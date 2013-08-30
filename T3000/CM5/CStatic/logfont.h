// LogFont.hxx: header file
//
// Declares CLogFont - implementation file is not defined for this class.
//
// Discussion:
//
//  This is a subclass of the LOGFONT structure and defines 
//  the attributes of a font. See the Microsoft documentation 
//  for more information.
//
// Modification History:
//
//  Bill Berry      March 1996      Creation
//
//  
// Copyright (C) Bill Berry 1996 
//
// This class may be used freely. Please distribute with its
// copyright notice in place.
//
#ifndef LOGFONT_H_E7F7D1A0_AE36_11d1_9257_006097CAB461
#define LOGFONT_H_E7F7D1A0_AE36_11d1_9257_006097CAB461

#ifndef __cplusplus
#   error C++ compilation is required.
#endif

struct CLogFont : public LOGFONT
{
    CLogFont( LONG fontHeight      = -8, 
              LONG fontWeight      = FW_NORMAL,
              UCHAR pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE,
            //  LPCSTR faceName      = _T("MS Sans Serif" )
			  LPCWSTR faceName      = _T("MS Sans Serif")
            )
    {
        const int size = sizeof(*this);

        memset( this, 0, size );

        lfHeight         = fontHeight;
        lfWeight         = fontWeight;
        lfPitchAndFamily = pitchAndFamily;

   //     ASSERT( LF_FACESIZE > lstrlen( faceName ) );
		wcscpy_s(lfFaceName,sizeof(lfFaceName),_T("MS Sans Serif"));
     //   lstrcpy( lfFaceName, faceName );
    }

    // Note: No need for CLogFont& operator =(const CLogFont& lf) {...}
    //       We let the compiler handle it...

    void lfFaceNameSet( LPCSTR faceName )
    {
        _ASSERT( faceName );

        if ( !faceName ) return;

//        _ASSERT( LF_FACESIZE > lstrlen( faceName ) );
	wcscpy_s(lfFaceName,sizeof(lfFaceName),_T("MS Sans Serif"));
    //    lstrcpy( lfFaceName, faceName );        
    }

};

#endif // LOGFONT_H_E7F7D1A0_AE36_11d1_9257_006097CAB461