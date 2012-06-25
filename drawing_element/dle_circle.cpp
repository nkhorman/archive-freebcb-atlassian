#include "stdafx.h"
#include "dle_circle.h"

extern CFreePcbApp theApp;

//-----------------------------------------------------------------------------
// filled circle
void CDLE_CIRC::_Draw(CDrawInfo &di, bool)
{
	if( onScreen() )
	{
		di.DC->Ellipse( i.x - w/2, i.y - w/2, i.x + w/2, i.y + w/2 );
	}
}

/* void CDLE_CIRC::_DrawClearance(CDrawInfo const &di) const
{
	int sz = w/2 + clearancew;

	di.DC->SelectObject( di.erase_brush );
	di.DC->SelectObject( di.erase_pen );

	di.DC->Ellipse( i.x - sz, i.y - sz, i.x + sz, i.y + sz );

	di.DC->SelectObject( di.fill_brush );
	di.DC->SelectObject( di.line_pen );
} */

void CDLE_CIRC::_DrawThermalRelief(CDrawInfo &di)
{
	CFreePcbDoc * doc = theApp.m_Doc;

	int conn_tracew = doc->m_thermal_width / 2540;
	int therm_clearance = doc->m_thermal_clearance / 2540;

	int sz = w/2 + therm_clearance;

	di.DC->SelectObject( di.erase_brush );
	di.DC->SelectObject( di.erase_pen );

	di.DC->Ellipse( i.x - sz, i.y - sz, i.x + sz, i.y + sz );

	di.DC->SelectObject( di.fill_brush );

	CPen pen( PS_SOLID, conn_tracew, di.layer_color[1] );
	sz -= (sz * 3) / 10; // sz *= 1/sqrt(2)

	di.DC->SelectObject( &pen );
	di.DC->MoveTo( i.x - sz, i.y + sz );
	di.DC->LineTo( i.x + sz, i.y - sz );
	di.DC->MoveTo( i.x - sz, i.y - sz );
	di.DC->LineTo( i.x + sz, i.y + sz );

	di.DC->SelectObject( di.line_pen );
}

int CDLE_CIRC::_IsHit(double x, double y, double &d)		// CPT r294, changed args and tidied up.
{
	d = Distance( i.x, i.y, x, y );
	return d < w/2.;
}



//-----------------------------------------------------------------------------
// hollow circle

void CDLE_HOLLOW_CIRC::_Draw(CDrawInfo &di, bool) 
{
	if( onScreen() )
	{
		di.DC->SelectObject( GetStockObject( HOLLOW_BRUSH ) );

		di.DC->Ellipse( i.x - w/2, i.y - w/2, i.x + w/2, i.y + w/2 );

		di.DC->SelectObject( di.fill_brush );
	}
}


int CDLE_HOLLOW_CIRC::_IsHit(CPoint &point)
{
	// test for hit (within 3 mils or 4 pixels)
//**	int dr = max( 3*DU_PER_MIL, int(4.0*m_scale) );
	int dr = 4.0 * dlist->m_scale;
	int hit_w = w/2;
	int d = Distance( i.x, i.y, point.x, point.y );

	return ( abs(hit_w-d) < dr );
}


