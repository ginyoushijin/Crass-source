#include "cxdec.h"

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage);
static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage);

static BYTE EncryptionControlBlock[4096] = {
		0x20, 0x45, 0x6e, 0x63, 0x72, 0x79, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x63, 0x6f, 0x6e, 0x74, 
		0x72, 0x6f, 0x6c, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 0x2d, 0x2d, 0x20, 0x53, 0x74, 0x61, 
		0x74, 0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x20, 0x6f, 0x72, 0x20, 0x64, 0x79, 0x6e, 0x61, 0x6d, 
		0x69, 0x63, 0x61, 0x6c, 0x6c, 0x79, 0x2c, 0x20, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 
		0x20, 0x6f, 0x72, 0x20, 0x69, 0x6e, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74, 0x6c, 0x79, 0x2c, 0x20, 
		0x75, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
		0x61, 0x6d, 0x20, 0x61, 0x6e, 0x64, 0x2f, 0x6f, 0x72, 0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x20, 
		0x66, 0x72, 0x6f, 0x6d, 0x20, 0x6f, 0x74, 0x68, 0x65, 0x72, 0x20, 0x70, 0x72, 0x6f, 0x67, 0x72, 
		0x61, 0x6d, 0x73, 0x20, 0x77, 0x69, 0x6c, 0x6c, 0x20, 0x62, 0x65, 0x20, 0x69, 0x6c, 0x6c, 0x65, 
		0x67, 0x61, 0x6c, 0x20, 0x62, 0x79, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6c, 0x69, 0x63, 0x65, 0x6e, 
		0x73, 0x65, 0x20, 0x61, 0x67, 0x72, 0x65, 0x65, 0x6d, 0x65, 0x6e, 0x74, 0x2e, 0x20, 0x82, 0xb1, 
		0x82, 0xcc, 0x83, 0x76, 0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xe2, 0x83, 0x75, 
		0x83, 0x8d, 0x83, 0x62, 0x83, 0x4e, 0x82, 0xf0, 0x81, 0x41, 0x90, 0xc3, 0x93, 0x49, 0x82, 0xc5, 
		0x82, 0xa0, 0x82, 0xea, 0x93, 0xae, 0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 
		0x92, 0xbc, 0x90, 0xda, 0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x8a, 0xd4, 0x90, 0xda, 
		0x93, 0x49, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xea, 0x81, 0x41, 0x91, 0xbc, 0x82, 0xcc, 0x83, 0x76, 
		0x83, 0x8d, 0x83, 0x4f, 0x83, 0x89, 0x83, 0x80, 0x82, 0xa9, 0x82, 0xe7, 0x97, 0x70, 0x82, 0xa2, 
		0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xcd, 0x83, 0x89, 0x83, 0x43, 0x83, 0x5a, 0x83, 0x93, 
		0x83, 0x58, 0x82, 0xc9, 0x82, 0xe6, 0x82, 0xe8, 0x8b, 0xd6, 0x82, 0xb6, 0x82, 0xe7, 0x82, 0xea, 
		0x82, 0xc4, 0x82, 0xa2, 0x82, 0xdc, 0x82, 0xb7, 0x81, 0x42, 0x0a, 0x81, 0x77, 0x31, 0x31, 0x65, 
		0x79, 0x65, 0x73, 0x81, 0x78, 0x82, 0xcc, 0x93, 0xc1, 0x8e, 0xea, 0x8b, 0x40, 0x94, 0x5c, 0x81, 
		0x46, 0x0a, 0x0a, 0x81, 0x9e, 0x83, 0x4e, 0x83, 0x8d, 0x83, 0x58, 0x83, 0x72, 0x83, 0x57, 0x83, 
		0x87, 0x83, 0x93, 0x83, 0x82, 0x81, 0x5b, 0x83, 0x68, 0x0a, 0x09, 0x8e, 0xe5, 0x90, 0x6c, 0x8c, 
		0xf6, 0x8e, 0x8b, 0x93, 0x5f, 0x88, 0xc8, 0x8a, 0x4f, 0x82, 0xc9, 0x91, 0xbc, 0x82, 0xcc, 0x93, 
		0x6f, 0x8f, 0xea, 0x90, 0x6c, 0x95, 0xa8, 0x82, 0xcc, 0x8e, 0x8b, 0x93, 0x5f, 0x82, 0xf0, 0x81, 
		0x75, 0x91, 0x7d, 0x93, 0xfc, 0x98, 0x62, 0x81, 0x76, 0x82, 0xcc, 0x8c, 0x60, 0x82, 0xc5, 0x8c, 
		0xa9, 0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xaa, 0x82, 0xc5, 0x82, 0xab, 0x82, 0xe9, 0x81, 
		0x42, 0x81, 0x75, 0x89, 0xf1, 0x91, 0x7a, 0x83, 0x82, 0x81, 0x5b, 0x83, 0x68, 0x81, 0x76, 0x8b, 
		0x40, 0x94, 0x5c, 0x82, 0xc9, 0x89, 0x65, 0x8b, 0xbf, 0x82, 0xf0, 0x97, 0x5e, 0x82, 0xa6, 0x82, 
		0xe9, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x9e, 0x83, 0x6f, 0x83, 0x62, 0x83, 0x68, 0x83, 0x47, 0x83, 
		0x93, 0x83, 0x68, 0x8b, 0x7e, 0x8d, 0xcf, 0x0a, 0x09, 0x83, 0x6f, 0x83, 0x62, 0x83, 0x68, 0x83, 
		0x47, 0x83, 0x93, 0x83, 0x68, 0x82, 0xf0, 0x89, 0xf1, 0x8e, 0xfb, 0x82, 0xb7, 0x82, 0xe9, 0x82, 
		0xc6, 0x82, 0xa2, 0x82, 0xa4, 0x8d, 0xec, 0x8b, 0xc6, 0x8a, 0xb4, 0x82, 0xf0, 0x8c, 0x79, 0x8c, 
		0xb8, 0x82, 0xb7, 0x82, 0xe9, 0x82, 0xbd, 0x82, 0xdf, 0x82, 0xc9, 0x81, 0x41, 0x83, 0x6f, 0x83, 
		0x62, 0x83, 0x68, 0x83, 0x47, 0x83, 0x93, 0x83, 0x68, 0x8e, 0x9e, 0x82, 0xc9, 0x83, 0x7e, 0x83, 
		0x6a, 0x83, 0x56, 0x83, 0x69, 0x83, 0x8a, 0x83, 0x49, 0x82, 0xf0, 0x91, 0x7d, 0x93, 0xfc, 0x82, 
		0xb7, 0x82, 0xe9, 0x82, 0xb1, 0x82, 0xc6, 0x82, 0xc6, 0x82, 0xb7, 0x82, 0xe9, 0x81, 0x42, 0x0a, 
		0x0a, 0x81, 0x9e, 0x89, 0xf1, 0x91, 0x7a, 0x83, 0x82, 0x81, 0x5b, 0x83, 0x68, 0x0a, 0x09, 0x81, 
		0x75, 0x83, 0x4e, 0x83, 0x8d, 0x83, 0x58, 0x83, 0x72, 0x83, 0x57, 0x83, 0x87, 0x83, 0x93, 0x83, 
		0x82, 0x81, 0x5b, 0x83, 0x68, 0x81, 0x76, 0x82, 0xc5, 0x8e, 0xe5, 0x90, 0x6c, 0x8c, 0xf6, 0x82, 
		0xcc, 0x8e, 0x8b, 0x93, 0x5f, 0x82, 0xaa, 0x93, 0xfc, 0x82, 0xe8, 0x8d, 0xac, 0x82, 0xb6, 0x82, 
		0xe9, 0x82, 0xbd, 0x82, 0xdf, 0x81, 0x41, 0x8e, 0x9e, 0x8c, 0x6e, 0x97, 0xf1, 0x82, 0xaa, 0x8d, 
		0xac, 0x97, 0x90, 0x82, 0xb5, 0x82, 0xc4, 0x82, 0xb5, 0x82, 0xdc, 0x82, 0xa4, 0x83, 0x86, 0x81, 
		0x5b, 0x83, 0x55, 0x81, 0x5b, 0x82, 0xe0, 0x82, 0xa2, 0x82, 0xe9, 0x82, 0xc6, 0x8e, 0x76, 0x82, 
		0xed, 0x82, 0xea, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x09, 0x82, 0xbb, 0x82, 0xb1, 0x82, 0xf0, 0x95, 
		0xe2, 0x8a, 0xae, 0x82, 0xb7, 0x82, 0xe9, 0x82, 0xbd, 0x82, 0xdf, 0x82, 0xcc, 0x94, 0x4e, 0x95, 
		0x5c, 0x8c, 0x5e, 0x82, 0xcc, 0x89, 0xf1, 0x91, 0x7a, 0x83, 0x82, 0x81, 0x5b, 0x83, 0x68, 0x81, 
		0x42, 0x0a, 0x0a, 0x81, 0x9e, 0x91, 0x8a, 0x8a, 0xd6, 0x90, 0x7d, 0x83, 0x82, 0x81, 0x5b, 0x83, 
		0x68, 0x0a, 0x09, 0x93, 0x6f, 0x8f, 0xea, 0x90, 0x6c, 0x95, 0xa8, 0x82, 0xcc, 0x8a, 0xd6, 0x8c, 
		0x57, 0x82, 0xf0, 0x83, 0x86, 0x81, 0x5b, 0x83, 0x55, 0x81, 0x5b, 0x82, 0xaa, 0x97, 0x9d, 0x89, 
		0xf0, 0x82, 0xb5, 0x82, 0xe2, 0x82, 0xb7, 0x82, 0xad, 0x82, 0xb7, 0x82, 0xe9, 0x82, 0xbd, 0x82, 
		0xdf, 0x82, 0xc9, 0x81, 0x41, 0x90, 0x6c, 0x8a, 0xd4, 0x8a, 0xd6, 0x8c, 0x57, 0x82, 0xcc, 0x81, 
		0x75, 0x91, 0x8a, 0x8a, 0xd6, 0x90, 0x7d, 0x81, 0x76, 0x82, 0xf0, 0x95, 0x5c, 0x8e, 0xa6, 0x82, 
		0xb7, 0x82, 0xe9, 0x83, 0x82, 0x81, 0x5b, 0x83, 0x68, 0x81, 0x42, 0x0a, 0x0a, 0x81, 0x9e, 0x83, 
		0x6e, 0x83, 0x43, 0x83, 0x75, 0x83, 0x8a, 0x83, 0x62, 0x83, 0x67, 0x83, 0x65, 0x83, 0x4c, 0x83, 
		0x58, 0x83, 0x67, 0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80, 0x0a, 0x09, 0x8b, 0xc2, 0x81, 
		0x58, 0x82, 0xb5, 0x82, 0xa2, 0x96, 0xbc, 0x91, 0x4f, 0x82, 0xc5, 0x82, 0xa0, 0x82, 0xe9, 0x82, 
		0xaa, 0x81, 0x41, 0x83, 0x81, 0x83, 0x62, 0x83, 0x5a, 0x81, 0x5b, 0x83, 0x57, 0x83, 0x45, 0x83, 
		0x42, 0x83, 0x93, 0x83, 0x68, 0x83, 0x45, 0x82, 0xc6, 0x83, 0x6d, 0x83, 0x78, 0x83, 0x8b, 0x83, 
		0x45, 0x83, 0x42, 0x83, 0x93, 0x83, 0x68, 0x83, 0x45, 0x82, 0xf0, 0x95, 0xb9, 0x97, 0x70, 0x82, 
		0xb7, 0x82, 0xe9, 0x83, 0x56, 0x83, 0x58, 0x83, 0x65, 0x83, 0x80, 0x81, 0x42, 0x0a, 0x0a, 0x81, 
		0x9e, 0x8f, 0xf3, 0x91, 0xd4, 0x82, 0xcc, 0x8e, 0xe6, 0x93, 0xbe, 0x0a, 0x09, 0x83, 0x56, 0x83, 
		0x58, 0x83, 0x65, 0x83, 0x80, 0x82, 0xcc, 0x83, 0x6f, 0x81, 0x5b, 0x83, 0x57, 0x83, 0x87, 0x83, 
		0x93, 0x8f, 0xee, 0x95, 0xf1, 0x82, 0xcc, 0x91, 0xbc, 0x82, 0xc9, 0x81, 0x41, 0x83, 0x86, 0x81, 
		0x5b, 0x83, 0x55, 0x81, 0x5b, 0x82, 0xcc, 0x82, 0x6f, 0x82, 0x62, 0x8a, 0xc2, 0x8b, 0xab, 0x82, 
		0xcc, 0x8f, 0xee, 0x95, 0xf1, 0x82, 0xf0, 0x8e, 0xe6, 0x93, 0xbe, 0x82, 0xc5, 0x82, 0xab, 0x82, 
		0xe9, 0x82, 0xe6, 0x82, 0xa4, 0x82, 0xc9, 0x82, 0xb7, 0x82, 0xe9, 0x81, 0x42, 0x83, 0x54, 0x83, 
		0x7c, 0x81, 0x5b, 0x83, 0x67, 0x8c, 0xfc, 0x82, 0xaf, 0x8b, 0x40, 0x94, 0x5c, 0x81, 0x42, 0x0a, 
		0x0a, 0x81, 0x9e, 0x93, 0xfc, 0x97, 0xcd, 0x90, 0xdd, 0x92, 0xe8, 0x0a, 0x09, 0x83, 0x7d, 0x83, 
		0x45, 0x83, 0x58, 0x81, 0x41, 0x83, 0x4c, 0x81, 0x5b, 0x83, 0x7b, 0x81, 0x5b, 0x83, 0x68, 0x81, 
		0x41, 0x83, 0x51, 0x81, 0x5b, 0x83, 0x80, 0x83, 0x70, 0x83, 0x62, 0x83, 0x68, 0x82, 0xcc, 0x93, 
		0xfc, 0x97, 0xcd, 0x90, 0xdd, 0x92, 0xe8, 0x82, 0xf0, 0x83, 0x86, 0x81, 0x5b, 0x83, 0x55, 0x81, 
		0x5b, 0x82, 0xcc, 0x94, 0x43, 0x88, 0xd3, 0x82, 0xc9, 0x95, 0xcf, 0x8d, 0x58, 0x82, 0xc5, 0x82, 
		0xab, 0x82, 0xe9, 0x82, 0xe6, 0x82, 0xa4, 0x82, 0xc9, 0x82, 0xb7, 0x82, 0xe9, 0x81, 0x42, 0x0a, 
		0x09, 0x4c, 0x61, 0x73, 0x73, 0x82, 0xcc, 0x82, 0xb1, 0x82, 0xea, 0x82, 0xdc, 0x82, 0xc5, 0x82, 
		0xcc, 0x83, 0x51, 0x81, 0x5b, 0x83, 0x80, 0x82, 0xb7, 0x82, 0xd7, 0x82, 0xc4, 0x82, 0xc9, 0x8e, 
		0xc0, 0x91, 0x95, 0x82, 0xb3, 0x82, 0xea, 0x82, 0xc4, 0x82, 0xa2, 0x82, 0xe9, 0x81, 0x42, 0x0a, 
		0x0a, 0x81, 0x9e, 0x8a, 0x6d, 0x94, 0x46, 0x83, 0x5f, 0x83, 0x43, 0x83, 0x41, 0x83, 0x8d, 0x83, 
		0x4f, 0x82, 0xcc, 0x83, 0x49, 0x83, 0x93, 0x83, 0x49, 0x83, 0x74, 0x0a, 0x09, 0x8a, 0x6d, 0x94, 
		0x46, 0x83, 0x5f, 0x83, 0x43, 0x83, 0x41, 0x83, 0x8d, 0x83, 0x4f, 0x82, 0xcc, 0x83, 0x7c, 0x83, 
		0x62, 0x83, 0x76, 0x83, 0x41, 0x83, 0x62, 0x83, 0x76, 0x83, 0x45, 0x83, 0x42, 0x83, 0x93, 0x83, 
		0x68, 0x83, 0x45, 0x82, 0xf0, 0x83, 0x86, 0x81, 0x5b, 0x83, 0x55, 0x81, 0x5b, 0x82, 0xaa, 0x94, 
		0x43, 0x88, 0xd3, 0x82, 0xc9, 0x95, 0x5c, 0x8e, 0xa6, 0x82, 0xb5, 0x82, 0xc8, 0x82, 0xa2, 0x82, 
		0xe6, 0x82, 0xa4, 0x82, 0xc9, 0x82, 0xc5, 0x82, 0xab, 0x82, 0xe9, 0x81, 0x42, 0x0a, 0x64, 0xd2, 
		0x7c, 0x6f, 0xe9, 0x26, 0x15, 0x9b, 0x87, 0x51, 0x2f, 0x3e, 0x88, 0x8b, 0x62, 0x7c, 0x57, 0xb8, 
		0x1c, 0x46, 0xbd, 0x33, 0xef, 0x0a, 0xd9, 0x80, 0xd2, 0x5b, 0x6f, 0xc0, 0xe0, 0x49, 0x1a, 0x20, 
		0xea, 0xa4, 0x2f, 0xaf, 0xed, 0x97, 0x16, 0x02, 0x4b, 0x2c, 0x30, 0x41, 0xcb, 0x06, 0x05, 0x04, 
		0x74, 0x0f, 0xc6, 0x86, 0x9f, 0x39, 0xfe, 0xf4, 0x6b, 0xac, 0xd4, 0x2f, 0xfb, 0xe2, 0xfd, 0x88, 
		0x1a, 0xb3, 0xab, 0x5e, 0xa9, 0x1d, 0xa3, 0xc2, 0x43, 0x43, 0x9b, 0x78, 0x30, 0xb9, 0x48, 0x81, 
		0x59, 0xd7, 0x5a, 0xa6, 0x00, 0xbe, 0xbe, 0xf1, 0x51, 0x0f, 0x52, 0x3f, 0x2c, 0xd0, 0xc7, 0x5c, 
		0x21, 0xe0, 0xf4, 0x64, 0x52, 0x02, 0x40, 0xf9, 0x1b, 0xf4, 0xcf, 0x27, 0xeb, 0x6a, 0x5b, 0x1c, 
		0xa0, 0x38, 0x29, 0xcd, 0x9c, 0x29, 0xce, 0x3b, 0x6d, 0x3e, 0x00, 0x62, 0x24, 0xd0, 0x03, 0x56, 
		0xe6, 0x83, 0x0b, 0xb9, 0x04, 0xfe, 0x64, 0xe5, 0xbc, 0xef, 0x90, 0x07, 0x6a, 0x3c, 0xde, 0x18, 
		0x74, 0x75, 0xf1, 0x1b, 0xd3, 0x8b, 0x64, 0xd6, 0xf8, 0x94, 0xb3, 0xcc, 0x41, 0x1f, 0x97, 0xf1, 
		0x69, 0xe4, 0xb2, 0xd4, 0xb3, 0xdf, 0x19, 0xc6, 0x76, 0x3e, 0x79, 0xb8, 0xe0, 0xe9, 0x85, 0x0a, 
		0x64, 0x11, 0x76, 0xd7, 0x17, 0xfb, 0x7f, 0xad, 0x44, 0x15, 0x1a, 0xb7, 0x25, 0x12, 0xa3, 0xde, 
		0x67, 0x91, 0x6b, 0x87, 0x52, 0x33, 0x6d, 0xa1, 0x59, 0x7b, 0xfb, 0xff, 0xc3, 0xff, 0x06, 0x5d, 
		0x83, 0xd3, 0xc1, 0xc8, 0x6e, 0xf2, 0x6d, 0x82, 0x64, 0x86, 0x37, 0xb0, 0x1b, 0xd0, 0x24, 0x8f, 
		0x37, 0xbb, 0x24, 0x71, 0x2b, 0x62, 0x82, 0x8f, 0x0b, 0x94, 0x17, 0xa3, 0xcc, 0x9f, 0xde, 0xcd, 
		0xa6, 0x58, 0x2c, 0x7a, 0x37, 0x02, 0x24, 0xd4, 0x7f, 0xed, 0x4d, 0x98, 0x7e, 0x95, 0xe6, 0x5c, 
		0x02, 0x3a, 0x8b, 0x0b, 0x1a, 0x92, 0x91, 0xbc, 0x86, 0x8d, 0xe5, 0xad, 0x49, 0x70, 0x5a, 0x9f, 
		0xca, 0x4f, 0x38, 0x0e, 0x4e, 0xf9, 0x9d, 0xbd, 0x91, 0x2a, 0xf5, 0x25, 0x6f, 0xef, 0x4a, 0xc1, 
		0xc2, 0xa8, 0x0e, 0x9c, 0x91, 0x00, 0xd2, 0x6a, 0x48, 0x3d, 0xff, 0xf0, 0x2e, 0x1a, 0x21, 0x55, 
		0xac, 0xbe, 0x39, 0xb7, 0x67, 0x82, 0xd2, 0xe4, 0x1b, 0xad, 0x9d, 0x5f, 0xa1, 0x84, 0x47, 0x3f, 
		0x0a, 0xe1, 0x06, 0xc7, 0x57, 0x4c, 0x93, 0xeb, 0x52, 0x40, 0x95, 0xbb, 0x7f, 0xdb, 0x16, 0x5a, 
		0x8f, 0x6c, 0xc7, 0xef, 0x4a, 0x05, 0x65, 0x0b, 0x6e, 0xbb, 0x76, 0x26, 0x6c, 0xc9, 0x55, 0x18, 
		0xae, 0x99, 0x8d, 0xaa, 0xb1, 0x1d, 0xb9, 0xac, 0x79, 0xf3, 0x69, 0x88, 0x89, 0xc5, 0xf8, 0x6d, 
		0x9a, 0xe4, 0x7e, 0xb0, 0x81, 0x80, 0xca, 0x83, 0x74, 0x2c, 0x03, 0xd8, 0x6f, 0xcc, 0x01, 0x25, 
		0x96, 0x95, 0xff, 0x7d, 0x40, 0x19, 0xa9, 0xd7, 0xa8, 0x0b, 0x12, 0x31, 0x26, 0x7c, 0xd5, 0x0e, 
		0x47, 0xd6, 0x8c, 0x9b, 0x2b, 0xd7, 0x3a, 0xb5, 0xd7, 0xae, 0x1f, 0x1d, 0xf6, 0x1b, 0xef, 0xd0, 
		0x00, 0x70, 0x20, 0x1b, 0xb3, 0xfd, 0x5c, 0xec, 0x8c, 0x4d, 0xfb, 0xd7, 0x8d, 0xfb, 0xd0, 0xd5, 
		0x16, 0x49, 0x88, 0x67, 0xbd, 0x40, 0x71, 0x49, 0xd8, 0x34, 0x35, 0x2c, 0xf7, 0xca, 0xc2, 0x1c, 
		0x2d, 0xd7, 0x44, 0xe1, 0x00, 0xda, 0xe3, 0x53, 0xe7, 0x55, 0x54, 0x1b, 0xf0, 0x4f, 0xa1, 0x9a, 
		0x89, 0x41, 0xc8, 0x48, 0x2c, 0x37, 0xbf, 0xf0, 0x0a, 0x6f, 0xc9, 0xd4, 0x0b, 0x80, 0xb4, 0x19, 
		0xae, 0xcd, 0xc7, 0x72, 0xe1, 0xd4, 0x1d, 0xb2, 0x1d, 0x52, 0x9e, 0xf6, 0x5b, 0x8b, 0xd3, 0x8b, 
		0x75, 0xde, 0xe8, 0xce, 0x44, 0xf7, 0x8a, 0xac, 0xd4, 0x3a, 0x5c, 0x75, 0x4d, 0xb9, 0x93, 0x72, 
		0x31, 0x17, 0xc6, 0x04, 0xa1, 0x16, 0xc5, 0x38, 0xab, 0x48, 0xb1, 0xb2, 0x6d, 0xc2, 0xc7, 0x3a, 
		0x16, 0x8c, 0xe8, 0x56, 0x34, 0x66, 0x87, 0x53, 0xb3, 0x44, 0x89, 0x05, 0xae, 0x04, 0xb0, 0x91, 
		0x9e, 0x40, 0xb1, 0xe1, 0x32, 0xc8, 0x51, 0xe3, 0xcb, 0xfd, 0xa8, 0x71, 0xd6, 0xee, 0x1e, 0x7d, 
		0xfc, 0x53, 0x8e, 0x98, 0x00, 0xb9, 0xc7, 0x10, 0x66, 0x6c, 0x7d, 0x8d, 0x63, 0x76, 0xf0, 0xe7, 
		0xf8, 0x25, 0xa0, 0x98, 0x1a, 0x7d, 0x13, 0x0a, 0xd1, 0x7c, 0xf1, 0x53, 0x9e, 0x1e, 0x51, 0x2e, 
		0xb8, 0x38, 0xdd, 0x49, 0xae, 0xdc, 0x17, 0xe2, 0x41, 0x66, 0x38, 0x2b, 0x20, 0xa5, 0x4d, 0xb6, 
		0xe3, 0x39, 0xfd, 0x3b, 0xa1, 0x2b, 0xab, 0x86, 0xe2, 0x25, 0xd1, 0x10, 0x4e, 0x22, 0xab, 0xfa, 
		0xe1, 0x17, 0xc6, 0xb6, 0xcb, 0x0a, 0x1c, 0x9d, 0x6b, 0x0d, 0x91, 0x0f, 0x06, 0x0c, 0x13, 0x98, 
		0x19, 0x42, 0x75, 0x7f, 0x25, 0x48, 0xcf, 0x72, 0x28, 0x69, 0x4d, 0x04, 0xc5, 0xa2, 0xbb, 0x63, 
		0x61, 0x73, 0x46, 0xd1, 0x63, 0x29, 0x4f, 0x97, 0x90, 0x4c, 0x1e, 0x30, 0x83, 0x8d, 0xe6, 0x76, 
		0x14, 0x19, 0x8a, 0x80, 0xd7, 0x52, 0x4c, 0xcf, 0xd0, 0x60, 0x3f, 0x0f, 0x75, 0x85, 0x96, 0x3f, 
		0x08, 0x8e, 0xe2, 0x0d, 0x2b, 0xde, 0xbc, 0xea, 0xaf, 0xe6, 0x7f, 0x05, 0x0c, 0x0a, 0xa9, 0x12, 
		0x98, 0x6d, 0x93, 0x1d, 0x6c, 0x59, 0xee, 0xc2, 0x5c, 0xf7, 0xa8, 0x53, 0xe4, 0xc9, 0xe1, 0xb8, 
		0x29, 0xef, 0x62, 0x22, 0x7b, 0x15, 0x85, 0x94, 0xad, 0xe0, 0x61, 0xde, 0x98, 0xea, 0x97, 0xff, 
		0xa6, 0xb0, 0x28, 0xb6, 0xed, 0xc1, 0x4a, 0x76, 0x31, 0xcb, 0xda, 0x88, 0x6c, 0x50, 0x28, 0xcb, 
		0xd8, 0xf4, 0xe1, 0x5d, 0x2d, 0x0a, 0x4c, 0x6b, 0xc0, 0x02, 0xdd, 0xd9, 0x73, 0xa9, 0x3d, 0x25, 
		0x29, 0x17, 0xb8, 0x03, 0xf8, 0x84, 0xc3, 0xd5, 0x0a, 0x4e, 0xed, 0x79, 0xeb, 0x00, 0x8c, 0xcb, 
		0x52, 0x8c, 0x9f, 0x9d, 0xa1, 0x60, 0xca, 0x83, 0xa6, 0x6a, 0x4a, 0x6f, 0x4e, 0x97, 0xbb, 0x40, 
		0xee, 0x7c, 0x86, 0xbd, 0x6e, 0x1e, 0x65, 0x61, 0xa4, 0x99, 0x7b, 0x85, 0x13, 0x41, 0x2f, 0x8d, 
		0x16, 0x7c, 0x31, 0x58, 0x39, 0xd1, 0x4d, 0x08, 0x1d, 0x47, 0xc4, 0x66, 0xae, 0x17, 0x49, 0x10, 
		0x83, 0xdf, 0x16, 0x32, 0xc2, 0x0f, 0x0f, 0x5f, 0x41, 0x87, 0x62, 0x1f, 0xac, 0x93, 0x48, 0xab, 
		0xa4, 0xcb, 0x6e, 0x95, 0x2d, 0x04, 0xa4, 0xe9, 0x69, 0x21, 0xcd, 0xde, 0x19, 0x12, 0x1e, 0x17, 
		0xc1, 0x81, 0xc3, 0x56, 0x0f, 0x6c, 0xe4, 0x66, 0xf6, 0x36, 0xd6, 0x81, 0xbe, 0xc4, 0x25, 0x21, 
		0x65, 0x46, 0x33, 0x22, 0x97, 0xb8, 0xb3, 0xe7, 0x21, 0x8e, 0xf9, 0x76, 0x6d, 0xf1, 0x08, 0x6f, 
		0xbd, 0xe8, 0xbb, 0x1f, 0x83, 0x1d, 0x09, 0x75, 0x3c, 0x1a, 0x2e, 0x62, 0x18, 0x3d, 0x13, 0xc5, 
		0x92, 0x5b, 0xed, 0x4d, 0x68, 0x10, 0xf4, 0x2a, 0x41, 0xfe, 0x63, 0x95, 0x19, 0x85, 0x6a, 0xc2, 
		0x24, 0x6d, 0xef, 0x46, 0x8d, 0x1f, 0xb8, 0x4c, 0x62, 0x39, 0x95, 0xa3, 0x1c, 0xb5, 0x75, 0x59, 
		0xec, 0x1f, 0xf4, 0x8b, 0x12, 0xb4, 0xe2, 0x1e, 0x9a, 0x20, 0x67, 0x33, 0xd3, 0x32, 0x28, 0xd1, 
		0x48, 0xb1, 0x26, 0x1a, 0xe5, 0xc1, 0xc1, 0x30, 0xba, 0xef, 0x79, 0x3a, 0x50, 0x7b, 0xc4, 0xf8, 
		0xb4, 0x6c, 0xde, 0x66, 0xdc, 0x4c, 0x8e, 0x30, 0xfe, 0x69, 0xb4, 0xde, 0x11, 0x1e, 0xbb, 0xa0, 
		0x8d, 0x92, 0xc8, 0x7e, 0x82, 0x6a, 0xf5, 0x39, 0x18, 0x2a, 0x9d, 0x9a, 0xce, 0xe1, 0x7c, 0xa0, 
		0x81, 0x5c, 0x85, 0xe7, 0x22, 0xe7, 0xeb, 0xa3, 0xc5, 0xa4, 0xa0, 0xb7, 0xdf, 0x46, 0xb2, 0x85, 
		0x9c, 0x18, 0x15, 0x67, 0x71, 0xd1, 0xbc, 0x54, 0x58, 0x2e, 0x99, 0x11, 0xf6, 0x3b, 0x29, 0x12, 
		0x4d, 0x60, 0x2b, 0xf2, 0x03, 0x2a, 0x84, 0x8e, 0x1b, 0x2b, 0x56, 0xf4, 0x6f, 0xe9, 0x9d, 0xbe, 
		0xb1, 0xed, 0x0e, 0x5f, 0x63, 0xc3, 0x5e, 0xc2, 0xa4, 0x03, 0x47, 0xbd, 0xe8, 0x43, 0x73, 0xe4, 
		0x67, 0x2c, 0xd6, 0xe9, 0xe2, 0xf7, 0xe4, 0x5d, 0x8f, 0x32, 0xc0, 0xdd, 0x84, 0xda, 0xa1, 0xc9, 
		0xdd, 0x00, 0x3a, 0x86, 0x9f, 0x11, 0xbb, 0x18, 0x91, 0xf1, 0x56, 0x12, 0x5a, 0x48, 0xfe, 0x91, 
		0xa2, 0x36, 0xc9, 0x44, 0xb5, 0x8e, 0xdc, 0xcc, 0x86, 0x7c, 0xd2, 0x50, 0x27, 0x51, 0x7a, 0xab, 
		0xb6, 0x02, 0x30, 0x8d, 0xad, 0x8f, 0x36, 0x3f, 0x04, 0x68, 0x49, 0x08, 0x27, 0x58, 0x86, 0x56, 
		0x56, 0x1c, 0x33, 0xa1, 0x98, 0x6a, 0x0f, 0x71, 0xe7, 0xa3, 0x8c, 0xbe, 0xa3, 0x59, 0xb1, 0xbb, 
		0xd3, 0xf9, 0xf1, 0x3a, 0x5b, 0xa5, 0xf5, 0x72, 0xe6, 0xfd, 0x37, 0x59, 0x3e, 0xdd, 0x2a, 0xf4, 
		0xaa, 0x1e, 0xd3, 0x9f, 0xbb, 0x9d, 0xf8, 0x61, 0x9d, 0x04, 0x0d, 0x5d, 0x59, 0x44, 0x7c, 0xa5, 
		0xa0, 0x30, 0x3f, 0x1a, 0x88, 0x90, 0x70, 0xf5, 0xf3, 0x06, 0xcc, 0xd4, 0xd0, 0x7e, 0xa0, 0x82, 
		0xb0, 0x40, 0xee, 0x22, 0x91, 0xa0, 0xd0, 0xf6, 0xe8, 0x40, 0xed, 0x65, 0x7d, 0xa2, 0x8e, 0xe9, 
		0x6e, 0xb3, 0x4e, 0x37, 0xbb, 0x22, 0xc9, 0xca, 0x81, 0x4f, 0x34, 0xac, 0xd4, 0x77, 0x1d, 0x9a, 
		0x7b, 0xc9, 0x8b, 0x24, 0x50, 0x6d, 0x2a, 0xf0, 0x1c, 0x1a, 0x5f, 0x65, 0xc7, 0xdd, 0x52, 0x5a, 
		0xe4, 0xb5, 0x28, 0xf8, 0x0c, 0x7c, 0x21, 0x5a, 0xa2, 0xb3, 0xb4, 0xba, 0xd2, 0xed, 0x1e, 0x18, 
		0xdd, 0xd6, 0x06, 0x2f, 0xc4, 0x00, 0x69, 0xe3, 0x51, 0x7e, 0xa8, 0x3d, 0xc3, 0x6e, 0xc3, 0x21, 
		0x29, 0x8c, 0x7b, 0x0e, 0xda, 0x5a, 0x1d, 0x64, 0x80, 0x7a, 0x23, 0x8a, 0xac, 0x84, 0x70, 0xbe, 
		0x56, 0x17, 0x5e, 0x65, 0x5b, 0xef, 0x86, 0x23, 0xb1, 0x19, 0xe0, 0xa3, 0x60, 0x28, 0xc3, 0xea, 
		0xe9, 0xd4, 0x4b, 0x88, 0x45, 0x78, 0x0a, 0x66, 0x9b, 0x68, 0xdd, 0xd9, 0xc1, 0xe5, 0x88, 0xf4, 
		0x22, 0x8f, 0xe9, 0xf9, 0x1a, 0x9e, 0x0e, 0x9d, 0x8d, 0xd9, 0xc4, 0x1f, 0xe6, 0x1c, 0xca, 0xa3, 
		0xa5, 0xe8, 0x2a, 0xc1, 0x82, 0x14, 0xb5, 0x77, 0xc1, 0x18, 0xcd, 0x0b, 0xc8, 0x1d, 0x5f, 0xa2, 
		0x84, 0x88, 0x3d, 0xb5, 0x47, 0x8a, 0x10, 0xff, 0x49, 0x87, 0x66, 0x10, 0x99, 0xf2, 0xce, 0x21, 
		0xc2, 0xfc, 0xc6, 0x6e, 0x9b, 0xd4, 0x70, 0x6e, 0x5c, 0xfd, 0xc9, 0x66, 0xda, 0xd0, 0x6d, 0x76, 
		0xa3, 0x0d, 0x78, 0xf3, 0x6e, 0xfe, 0xb1, 0xcb, 0x0c, 0x29, 0x4b, 0x56, 0x17, 0x36, 0x5e, 0x8c, 
		0x79, 0xd5, 0x4a, 0x44, 0x1b, 0xc2, 0xb2, 0x79, 0x92, 0x84, 0x68, 0xc8, 0xe8, 0x12, 0x53, 0x32, 
		0xf9, 0x4c, 0xc7, 0x56, 0xac, 0x60, 0xfa, 0x69, 0x11, 0x0d, 0xba, 0x97, 0x10, 0xf8, 0x1c, 0xce, 
		0x07, 0x8d, 0x7a, 0x3c, 0xe6, 0x69, 0x45, 0x28, 0x23, 0xb9, 0x34, 0x4f, 0xe2, 0x0c, 0xb6, 0xab, 
		0x86, 0x9c, 0x40, 0x00, 0x78, 0x95, 0x87, 0x83, 0x69, 0xa4, 0xe1, 0x8c, 0x79, 0xca, 0xad, 0x69, 
		0xab, 0xd2, 0x2a, 0x66, 0xbe, 0x8b, 0x38, 0x51, 0x9f, 0xf3, 0x0c, 0x1d, 0x40, 0x4c, 0xbb, 0xcd, 
		0xcc, 0x95, 0xbb, 0xeb, 0x45, 0x07, 0xf8, 0x48, 0xa7, 0x83, 0x82, 0x83, 0x89, 0x55, 0xc9, 0x21, 
		0x7b, 0x28, 0x36, 0xe4, 0x3f, 0xb9, 0x1e, 0x4b, 0x9d, 0xa5, 0x72, 0xac, 0x48, 0xb3, 0x2e, 0xc4, 
		0x20, 0x10, 0xd2, 0x61, 0x61, 0x85, 0xb3, 0x3b, 0xe3, 0x84, 0x61, 0xdc, 0x0c, 0x13, 0xc0, 0x8a, 
		0x1f, 0xd3, 0xba, 0xec, 0x85, 0x30, 0x1b, 0x45, 0x05, 0x40, 0x3e, 0x44, 0xad, 0x38, 0x60, 0x10, 
		0x3e, 0x41, 0x07, 0x85, 0xaa, 0xf3, 0x1e, 0xb3, 0x88, 0x42, 0x55, 0x0c, 0xf3, 0x95, 0x61, 0x24, 
		0x84, 0xdd, 0x29, 0xc8, 0x81, 0x69, 0xbb, 0x3d, 0xd8, 0x0a, 0x0c, 0x88, 0x1f, 0xd5, 0xa2, 0x1b, 
		0xb6, 0x1a, 0x1e, 0x49, 0x93, 0x74, 0xb7, 0xda, 0x97, 0xb0, 0x4f, 0x1e, 0xc1, 0x3e, 0x12, 0xae, 
		0x2b, 0x26, 0x17, 0xd7, 0x39, 0x76, 0x9d, 0x0a, 0xd3, 0x70, 0xc8, 0x18, 0x94, 0x04, 0xed, 0x0c, 
		0x99, 0x52, 0x9c, 0xf7, 0x64, 0x07, 0xde, 0x2f, 0x52, 0x51, 0x3c, 0xae, 0xc0, 0x85, 0x1c, 0xea, 
		0x37, 0x1d, 0x71, 0x8a, 0x76, 0xfd, 0xe2, 0xd4, 0xcf, 0x20, 0x07, 0x2c, 0x58, 0x5a, 0x0b, 0x91, 
		0xfe, 0xc6, 0xf0, 0xdb, 0xe8, 0x67, 0x88, 0x05, 0xe0, 0x80, 0x23, 0xee, 0xfb, 0x67, 0x5f, 0x71, 
		0xe3, 0x0a, 0x63, 0x1a, 0xc5, 0x64, 0xf2, 0xc9, 0x75, 0x0e, 0x30, 0x21, 0xb6, 0x3b, 0x8e, 0xad, 
		0x4f, 0xf6, 0x18, 0x04, 0xbd, 0xe8, 0xac, 0x34, 0xd0, 0x5a, 0x9c, 0x54, 0x11, 0x62, 0x39, 0xaf, 
		0xaa, 0x47, 0x6f, 0xbe, 0x45, 0x38, 0x86, 0x57, 0xc1, 0x00, 0xed, 0xd5, 0xd2, 0xa1, 0x87, 0x34, 
		0x8d, 0xa7, 0x9a, 0x17, 0x97, 0x40, 0x08, 0xd4, 0x1f, 0xbf, 0x14, 0xd7, 0x79, 0x8b, 0xd3, 0x62, 
		0x4e, 0x82, 0xec, 0xd5, 0xbe, 0xdb, 0x2e, 0x55, 0xc7, 0xd0, 0xee, 0x6c, 0xb2, 0x0a, 0x9e, 0x52, 
		0x77, 0xe0, 0x3c, 0xcd, 0x3d, 0xab, 0xab, 0xe5, 0xd1, 0x33, 0xce, 0x47, 0x29, 0x47, 0xd3, 0x20, 
		0xd2, 0x7b, 0x73, 0x54, 0x08, 0x61, 0xba, 0x6c, 0x5b, 0x30, 0xa4, 0x9c, 0xb1, 0x76, 0xde, 0x82, 
		0xe7, 0xfd, 0x1b, 0x71, 0x4b, 0xe5, 0xca, 0x3c, 0xce, 0x13, 0xcc, 0xc2, 0xbc, 0x95, 0x78, 0x50, 
		0x9f, 0xfc, 0xf1, 0x2e, 0x41, 0xe2, 0x53, 0x87, 0xe8, 0x11, 0x58, 0xb1, 0xab, 0x4b, 0xe8, 0x49, 
		0xd5, 0x6e, 0x75, 0x64, 0xdf, 0xc3, 0x1e, 0x6c, 0xd1, 0x37, 0x68, 0xc0, 0x75, 0xc1, 0x63, 0x5e, 
		0x03, 0xcb, 0xaa, 0x2f, 0x4a, 0xda, 0xf1, 0x28, 0xab, 0x2a, 0x73, 0x8a, 0xd4, 0x05, 0xde, 0x6a, 
		0xd3, 0x78, 0xe1, 0x1a, 0x6c, 0xea, 0xe2, 0x23, 0x1c, 0x14, 0xcf, 0x09, 0xc8, 0x31, 0x4a, 0x79, 
		0x0d, 0x19, 0xa2, 0x25, 0x16, 0x2a, 0x24, 0xe3, 0x34, 0x56, 0xb6, 0x9a, 0xf1, 0x5d, 0x77, 0x90, 
		0x30, 0xa3, 0x34, 0x1c, 0xfb, 0x15, 0x78, 0xce, 0xbf, 0xec, 0x72, 0xbb, 0xeb, 0x9b, 0x65, 0x66, 
		0x14, 0x27, 0x34, 0x23, 0xf2, 0xb8, 0xdd, 0xe2, 0x2c, 0x15, 0x22, 0x1b, 0x0e, 0xe9, 0x81, 0xb9, 
		0x55, 0xb9, 0xcd, 0x05, 0x56, 0xd3, 0x93, 0x30, 0x65, 0x9b, 0x95, 0xf8, 0x3c, 0x7d, 0x83, 0x7b, 
		0xaf, 0xfa, 0x0e, 0x9c, 0xa7, 0x6a, 0x24, 0x92, 0x7a, 0x2a, 0x40, 0xc4, 0xc6, 0xca, 0x48, 0xa5, 
		0xfb, 0xab, 0xc9, 0x38, 0x5a, 0x45, 0xad, 0xf0, 0xf7, 0xfb, 0x22, 0xa3, 0x2a, 0x93, 0x06, 0xa6, 
		0x8a, 0x6d, 0x21, 0x2e, 0xd9, 0x3f, 0x34, 0xe2, 0x9d, 0x9b, 0x28, 0x77, 0x8f, 0x7f, 0x31, 0x2d, 
		0x63, 0x8e, 0x1d, 0x95, 0x0e, 0x14, 0xc2, 0xf9, 0x77, 0x3f, 0xd6, 0x37, 0xea, 0x42, 0x51, 0x64, 
		0x75, 0x1f, 0xb3, 0xf6, 0x8b, 0x85, 0x49, 0x18, 0x66, 0xc4, 0xdb, 0x28, 0x5d, 0x41, 0x35, 0xe9, 
		0x4e, 0x3a, 0xe0, 0x27, 0xb8, 0xe8, 0x60, 0xba, 0x37, 0xbc, 0x62, 0x41, 0x74, 0xc4, 0xde, 0xcc, 
		0x59, 0x70, 0xaf, 0xd7, 0x99, 0x23, 0x47, 0x1b, 0xac, 0xc4, 0x1d, 0xcb, 0xfa, 0xe3, 0x4d, 0xc5, 
		0x52, 0x4a, 0x00, 0x8b, 0x51, 0x02, 0xaf, 0x75, 0x5f, 0x0b, 0x3a, 0x62, 0xd4, 0x51, 0xbc, 0x34, 
		0xd9, 0xe7, 0x52, 0xc6, 0xde, 0xd0, 0xe3, 0x3a, 0x96, 0x4c, 0xf1, 0x2f, 0x44, 0x1b, 0x09, 0x16, 
		0x6b, 0x2f, 0xab, 0x11, 0xf9, 0xf2, 0x24, 0x41, 0xac, 0x08, 0xb7, 0x4f, 0xb9, 0xba, 0xfc, 0x46, 
		0xb6, 0xae, 0x1e, 0xcd, 0x8c, 0xd9, 0x9a, 0x36, 0x33, 0xf6, 0x45, 0x6f, 0xae, 0x22, 0x8d, 0x71, 
		0xe8, 0x1d, 0x66, 0x1a, 0x76, 0xe0, 0x0f, 0x88, 0x9b, 0xe5, 0xc6, 0xd1, 0x69, 0x53, 0xc2, 0x15, 
		0xff, 0xb3, 0x1a, 0x0c, 0xe8, 0x8f, 0x37, 0xc2, 0x17, 0x5b, 0x49, 0x03, 0xfe, 0x36, 0x4e, 0x6e, 
		0x9a, 0x0b, 0xd7, 0x32, 0x0b, 0xc6, 0x7e, 0x8b, 0x42, 0x97, 0x28, 0x47, 0x6b, 0x73, 0x17, 0xf4, 
		0xc9, 0xa8, 0x0e, 0xe5, 0x7b, 0x45, 0xe1, 0xf8, 0x44, 0x9c, 0xeb, 0x60, 0xfe, 0x29, 0x9d, 0x7c, 
		0x59, 0x95, 0x7d, 0xe5, 0xd6, 0x33, 0xd5, 0x5a, 0x08, 0x25, 0x72, 0x21, 0x0d, 0x55, 0x9b, 0xba, 
		0xac, 0x14, 0x99, 0x3c, 0x13, 0xe5, 0x79, 0x93, 0xa4, 0x69, 0x07, 0x41, 0xcc, 0x1f, 0x83, 0xdd, 
		0x4f, 0xfc, 0xc7, 0x50, 0x86, 0x88, 0x4e, 0xde, 0x28, 0x80, 0xfe, 0x15, 0x2c, 0x99, 0xee, 0x93, 
		0x14, 0x41, 0xfd, 0x9e, 0x0f, 0xab, 0xbd, 0x26, 0x88, 0x4c, 0x85, 0x76, 0x9a, 0x52, 0xe8, 0x26, 
		0x3c, 0xc4, 0xe6, 0x3b, 0x0d, 0x3e, 0x34, 0x82, 0xf3, 0x16, 0x27, 0x5b, 0xce, 0xe3, 0xf6, 0x7f, 
		0x54, 0x89, 0xd0, 0x28, 0xf6, 0xe8, 0x3b, 0x46, 0xfe, 0x5e, 0xd6, 0x61, 0x5f, 0x81, 0x81, 0xc1, 
		0xa0, 0x34, 0x76, 0xed, 0x23, 0xcf, 0x6a, 0x73, 0xfa, 0xdd, 0xde, 0x82, 0x8c, 0xda, 0x1c, 0xd1, 
		0x0f, 0xb4, 0x6b, 0x7c, 0xe1, 0xc0, 0x94, 0xc9, 0xae, 0x10, 0x2b, 0x7b, 0x24, 0x6c, 0x4a, 0x6c, 
		0x94, 0xea, 0x20, 0x72, 0x12, 0xb9, 0xd1, 0xf5, 0xe8, 0x5e, 0x7a, 0xe9, 0xc1, 0xba, 0xdc, 0xe3, 
		0xee, 0x04, 0x7c, 0x12, 0x2c, 0x6b, 0x00, 0x1f, 0xc1, 0xe2, 0x0b, 0x4e, 0x98, 0xb8, 0x92, 0xb9, 
		0xd0, 0x93, 0x96, 0xea, 0x7b, 0x0e, 0xe3, 0x5e, 0x37, 0x75, 0x61, 0xc8, 0x8d, 0x91, 0x19, 0x4e, 
		0x1e, 0x45, 0x06, 0x32, 0x7b, 0xa7, 0xb8, 0xc6, 0xa3, 0x2f, 0xa1, 0x7a, 0x7d, 0x6f, 0x4d, 0xbf, 
		0xa7, 0xb6, 0x99, 0x3b, 0x00, 0x34, 0xad, 0x9a, 0x67, 0x95, 0x03, 0x25, 0xd5, 0x85, 0xc9, 0x12, 
		0x1e, 0xd4, 0xcf, 0xfc, 0x7f, 0xb4, 0x0b, 0xd5, 0x69, 0x0f, 0xee, 0xb1, 0xd9, 0x21, 0xf4, 0xd5, 
		0x27, 0x1a, 0xd4, 0x4b, 0x17, 0x28, 0x3e, 0xa5, 0x4a, 0x81, 0x8f, 0x61, 0x10, 0xbd, 0xe4, 0x06, 
		0xd5, 0xe4, 0xef, 0x12, 0xc5, 0x35, 0x5c, 0x24, 0xb5, 0x62, 0xb2, 0xbc, 0xf7, 0xdc, 0x7e, 0xba, 
		0xfa, 0xcb, 0xb2, 0x27, 0x1d, 0x35, 0xbc, 0x95, 0xcb, 0x13, 0x0a, 0x5b, 0xdb, 0xe2, 0x77, 0xbe, 
		0xfa, 0x39, 0xa5, 0x5a, 0xd8, 0xe5, 0x90, 0xde, 0x74, 0x5b, 0x05, 0x27, 0x65, 0xc8, 0x8f, 0x81, 
		0x19, 0x78, 0x12, 0x3f, 0xc1, 0x41, 0x5e, 0xc0, 0x92, 0x79, 0xce, 0xb9, 0xe6, 0x05, 0x26, 0x68, 
		0xcb, 0xef, 0x03, 0x9c, 0x19, 0xbe, 0x23, 0x97, 0x51, 0xc1, 0x53, 0xfe, 0xc3, 0x0e, 0xc9, 0xae, 
		0x84, 0x76, 0x29, 0x4a, 0x35, 0x32, 0x62, 0x00, 0x61, 0x23, 0x76, 0x34, 0x26, 0xcc, 0x14, 0xc2, 
		0x88, 0x33, 0xe0, 0xca, 0x7f, 0x94, 0x72, 0xa2, 0x8f, 0x30, 0x4a, 0xf7, 0xd8, 0xda, 0x51, 0x9b, 
		0x3d, 0x5d, 0xce, 0x7c, 0x01, 0xf0, 0x96, 0x84, 0x4c, 0xc5, 0xf4, 0x19, 0x21, 0x1a, 0xfa, 0xa2, 
		0x45, 0x88, 0x95, 0x55, 0x10, 0xf8, 0xf1, 0x58, 0x93, 0xd5, 0x9e, 0xd9, 0x00, 0x3e, 0x75, 0x07, 
		0x19, 0x89, 0x2d, 0x65, 0x40, 0x7c, 0x30, 0xcf, 0x38, 0x39, 0xdd, 0xca, 0x35, 0x2f, 0xa8, 0x1f, 
		0x2c, 0xb4, 0x3f, 0xf3, 0xfc, 0x93, 0x95, 0x65, 0xd8, 0x34, 0x93, 0xed, 0x59, 0xb2, 0xd4, 0x73, 
		0x54, 0xab, 0xb7, 0x62, 0x52, 0xa7, 0xf9, 0xb7, 0x24, 0x2e, 0x1a, 0x3b, 0x42, 0x88, 0x6e, 0xd4, 
		0x29, 0xc0, 0x50, 0x1f, 0x34, 0xf4, 0x9a, 0xcb, 0x19, 0x95, 0x56, 0xd6, 0xc3, 0x2d, 0x52, 0x34, 
		0x7d, 0x36, 0xae, 0x2b, 0x28, 0xd9, 0x3c, 0x67, 0x4d, 0xd1, 0xd7, 0xf7, 0xfb, 0x38, 0x30, 0x00, 
		0x3c, 0x0b, 0xdf, 0x56, 0x5c, 0x2a, 0x3c, 0x5e, 0x2b, 0x02, 0xa6, 0x35, 0x35, 0xad, 0x55, 0x05, 
		0xa7, 0xe1, 0xfb, 0x75, 0x0c, 0x4b, 0x8a, 0x5f, 0x08, 0x6b, 0x47, 0x99, 0x2a, 0xb8, 0x39, 0x49, 
		0x89, 0x06, 0x56, 0x3a, 0x3d, 0xc8, 0x4f, 0xc7, 0xb1, 0x29, 0x46, 0x40, 0xca, 0xc4, 0xd8, 0xe4, 
		0x69, 0x89, 0xda, 0x8d, 0xc7, 0xcd, 0x78, 0xf1, 0x7d, 0x43, 0xac, 0x06, 0x59, 0x84, 0x67, 0x16, 
		0x4c, 0x78, 0x62, 0xd3, 0x46, 0x48, 0x89, 0x83, 0xd7, 0x10, 0xbc, 0xd5, 0x30, 0xd6, 0x6d, 0x52, 
		0x2b, 0x2f, 0x7c, 0x65, 0x79, 0x80, 0xae, 0xc3, 0xd6, 0x75, 0xb1, 0xa0, 0x29, 0x19, 0x9a, 0xd2, 
		0xfe, 0xba, 0x36, 0xeb, 0xe5, 0xb9, 0x35, 0x93, 0xc9, 0x3b, 0x63, 0x53, 0xd8, 0x5f, 0xf3, 0x25, 
		0x6e, 0x96, 0xd2, 0x70, 0x55, 0x1b, 0x37, 0x02, 0x14, 0xc4, 0xc3, 0x70, 0xdf, 0xdc, 0xf9, 0xbe, 
};

static int xcode_building_first_stage(struct cxdec_xcode_status *xcode)
{
  	switch (xcode_rand(xcode) % 3) {
  	case 2:
		// MOV ESI, EncryptionControlBlock
		// MOV EAX, DWORD PTR DS:[ESI+((xcode_rand(xcode) & 0x3ff) << 2)]
		if (!push_bytexcode(xcode, 0xbe)
				|| !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
				|| !push_2bytesxcode(xcode, 0x8b, 0x86)
				|| !push_dwordxcode(xcode, (xcode_rand(xcode) & 0x3ff) << 2))
			return 0;
		break;
	case 1:
  		// MOV EAX, EDI
  		if (!push_2bytesxcode(xcode, 0x8b, 0xc7))
  			return 0;
		break;
  	case 0:
		// MOV EAX, xcode_rand(xcode)
		if (!push_bytexcode(xcode, 0xb8)
				|| !push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
  		break;
  	}
  	return 1;
}

static int xcode_building_stage0(struct cxdec_xcode_status *xcode, int stage)
{
	if (stage == 1)
		return xcode_building_first_stage(xcode);

    if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
    } else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
    }

    switch (xcode_rand(xcode) & 7) {
	case 7:
		// NOT EAX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd0))
			return 0;
        break;
	case 3:
		// MOV ESI, EncryptionControlBlock
		// AND EAX, 3FFh
		// MOV EAX, DWORD PTR DS:[ESI+EAX*4]
        if (!push_bytexcode(xcode, 0xbe)
  				|| !push_dwordxcode(xcode, (DWORD)EncryptionControlBlock)
				|| !push_bytexcode(xcode, 0x25)
				|| !push_dwordxcode(xcode, 0x3ff)
				|| !push_3bytesxcode(xcode, 0x8b, 0x04, 0x86))
			return 0;
        break;
	case 0:
		// DEC EAX
		if (!push_bytexcode(xcode, 0x48))
			return 0;
        break;
	case 6:
		// NEG EAX
        if (!push_2bytesxcode(xcode, 0xf7, 0xd8))
			return 0;
		break;
	case 4:
        if (xcode_rand(xcode) & 1) {
        	// ADD EAX, xcode_rand(xcode)
			if (!push_bytexcode(xcode, 0x05))
            	return 0;
        } else {
        	// SUB EAX, xcode_rand(xcode)
			if (!push_bytexcode(xcode, 0x2d))
            	return 0;
        }
        if (!push_dwordxcode(xcode, xcode_rand(xcode)))
        	return 0;
        break;
	case 5:
		// PUSH EBX
		// MOV EBX, EAX
		// AND EBX, AAAAAAAA
		// AND EAX, 55555555
		// SHR EBX, 1
		// SHL EAX, 1
		// OR EAX, EBX
		// POP EBX
		if (!push_bytexcode(xcode, 0x53)
				|| !push_2bytesxcode(xcode, 0x89, 0xc3)
				|| !push_6bytesxcode(xcode, 0x81, 0xe3, 0xaa, 0xaa, 0xaa, 0xaa)
				|| !push_5bytesxcode(xcode, 0x25, 0x55, 0x55, 0x55, 0x55)
				|| !push_2bytesxcode(xcode, 0xd1, 0xeb)
				|| !push_2bytesxcode(xcode, 0xd1, 0xe0)
				|| !push_2bytesxcode(xcode, 0x09, 0xd8)
				|| !push_bytexcode(xcode, 0x5b))
			return 0;
		break;
	case 2:
		// INC EAX
		if (!push_bytexcode(xcode, 0x40))
			return 0;
		break;
	case 1:
		// XOR EAX, xcode_rand(xcode)
		if (!push_bytexcode(xcode, 0x35) 
				|| !push_dwordxcode(xcode, xcode_rand(xcode)))
			return 0;
        break;
	}
	return 1;
}

static int xcode_building_stage1(struct cxdec_xcode_status *xcode, int stage)
{
	if (stage == 1)
		return xcode_building_first_stage(xcode);

	// PUSH EBX
	if (!push_bytexcode(xcode, 0x53))
		return 0;

	if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
	} else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
	}

	// MOV EBX, EAX
	if (!push_2bytesxcode(xcode, 0x89, 0xc3))
		return 0;

	if (xcode_rand(xcode) & 1) {
		if (!xcode_building_stage1(xcode, stage - 1))
			return 0;
	} else {
		if (!xcode_building_stage0(xcode, stage - 1))
			return 0;
	}

	switch (xcode_rand(xcode) % 6) {
	case 2:
		// ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
    case 0:
		// PUSH ECX
		// MOV ECX, EBX
		// AND ECX, 0F
		// SHR EAX, CL
		// POP ECX
		if (!push_bytexcode(xcode, 0x51)
				|| !push_2bytesxcode(xcode, 0x89, 0xd9)
				|| !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
				|| !push_2bytesxcode(xcode, 0xd3, 0xe8)
				|| !push_bytexcode(xcode, 0x59))
			return 0;
		break;
    case 1:
    	// PUSH ECX
    	// MOV ECX, EBX
    	// AND ECX, 0F
    	// SHL EAX, CL
    	// POP ECX
		if (!push_bytexcode(xcode, 0x51) 
				|| !push_2bytesxcode(xcode, 0x89, 0xd9)
				|| !push_3bytesxcode(xcode, 0x83, 0xe1, 0x0f)
				|| !push_2bytesxcode(xcode, 0xd3, 0xe0)
				|| !push_bytexcode(xcode, 0x59))
			return 0;
		break;
    case 4:
    	// NEG EAX, ADD EAX, EBX
		if (!push_2bytesxcode(xcode, 0xf7, 0xd8) 
				|| !push_2bytesxcode(xcode, 0x01, 0xd8))
			return 0;
		break;
    case 5:
    	// IMUL EAX, EBX
		if (!push_3bytesxcode(xcode, 0x0f,  0xaf, 0xc3))
			return 0;
		break;
    case 3:
    	// SUB EAX, EBX
		if (!push_2bytesxcode(xcode, 0x29, 0xd8))
			return 0;
		break;
	}
	// POP EBX
	return push_bytexcode(xcode, 0x5b);
}

struct cxdec_callback _11eyes_cxdec_callback = {
	"11eyes",
	{ 0x1eb, 0x1ef },
	xcode_building_stage1
};
