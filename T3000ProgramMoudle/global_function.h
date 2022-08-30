#pragma once

#include "pch.h"
#include "dllmain.h"
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include <stdint.h>
#include <stdbool.h>
#include "global_define.h"
#include "../T3000/ProductModel.h"

CString GetProductName(int ModelID);
void Inial_Product_map();