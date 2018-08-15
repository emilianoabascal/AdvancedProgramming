#ifndef NUMERICAL_BASE_CONVERSION_FUNCS_H
#define NUMERICAL_BASE_CONVERSION_FUNCS_H

void fromAnyBaseToDecimal(char *number, char *base1, char *base2);
int getNumberOfChars(char *number);
void getOtherNumbers(char *numberList, int arraySize);
void fromDecimalToOtherBases(int n, char *base);

#endif