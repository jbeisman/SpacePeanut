
#pragma once

#include <vector>
#include <array>

struct GridConst {
	const float CLEN;
	const int NGRID;
	const float GMAX;
	const float GMIN;
	const int NGRID_HALF;
	const float CLEN_HALF;
	const float CLEN_INV;
	const float CVOL_INV;
	GridConst() = delete;
	GridConst(int ngrid, float gmax)
	:	CLEN(gmax / ngrid),
		NGRID(ngrid),
		GMAX(gmax),
		GMIN(0.0f),
		NGRID_HALF(ngrid / 2 + 1),
		CLEN_HALF(CLEN / 2),
		CLEN_INV(1.0 / CLEN),
		CVOL_INV(1.0 / (CLEN*CLEN*CLEN))
		{ }
};
