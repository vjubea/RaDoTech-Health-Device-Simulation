#ifndef DEFS_H
#define DEFS_H

#include<QString>

// Hand (R/L) Reading Good Ranges
#define H1_GoodReadSTART 58
#define H1_GoodReadEND 83

#define H2_GoodReadSTART 53
#define H2_GoodReadEND 77

#define H3_GoodReadSTART 43
#define H3_GoodReadEND 63

#define H4_GoodReadSTART 53
#define H4_GoodReadEND 77

#define H5_GoodReadSTART 60
#define H5_GoodReadEND 90

#define H6_GoodReadSTART 61
#define H6_GoodReadEND 89

const int HandGoodReadStart[] = {
    H1_GoodReadSTART, H2_GoodReadSTART, H3_GoodReadSTART,
    H4_GoodReadSTART, H5_GoodReadSTART, H6_GoodReadSTART
};
const int HandGoodReadEnd[] = {
    H1_GoodReadEND, H2_GoodReadEND, H3_GoodReadEND,
    H4_GoodReadEND, H5_GoodReadEND, H6_GoodReadEND
};

// Feet (R/L) Reading Good Ranges
#define F1_GoodReadSTART 48
#define F1_GoodReadEND 71

#define F2_GoodReadSTART 40
#define F2_GoodReadEND 58

#define F3_GoodReadSTART 45
#define F3_GoodReadEND 64

#define F4_GoodReadSTART 45
#define F4_GoodReadEND 65

#define F5_GoodReadSTART 40
#define F5_GoodReadEND 57

#define F6_GoodReadSTART 43
#define F6_GoodReadEND 64

const int FootGoodReadStart[] = {
    F1_GoodReadSTART, F2_GoodReadSTART, F3_GoodReadSTART,
    F4_GoodReadSTART, F5_GoodReadSTART, F6_GoodReadSTART
};

const int FootGoodReadEnd[] = {
    F1_GoodReadEND, F2_GoodReadEND, F3_GoodReadEND,
    F4_GoodReadEND, F5_GoodReadEND, F6_GoodReadEND
};


const QString organs[] = {
    "Lungs", "Pericardium", "Heart", "Small Intestine", "Lymph", "Large Intestine",
    "Spleen & Pancreas", "Liver", "Kidney", "Bladder", "Gall Bladder", "Stomach"
};

#endif // DEFS_H
