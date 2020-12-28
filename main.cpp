#include <iostream>
#include <stdint.h>
#include <ctime>

void vHammingAdd_2(uint8_t *,uint8_t *);
void vHammingCheck_2(uint8_t *, uint8_t *);
void vCorruptedSending(uint8_t *, uint8_t *, uint8_t);

using namespace std;

int main()
{
    uint8_t stringToSend[21] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',1,1,1,1,1};
    uint8_t stringRecieved[21];
    srand(time(NULL));
    vHammingAdd_2(stringToSend, &stringToSend[16]);

    vCorruptedSending(stringToSend, stringRecieved, 21);

    vHammingCheck_2(stringRecieved, &stringRecieved[16]);
    cout << endl;
    for(int i = 0; i < 16; i++){
        cout << stringRecieved[i] ;
    }
    cout << endl;
    return 0;
}



void vCorruptedSending(uint8_t *pPackage, uint8_t *pTarget, uint8_t size)
{
    memcpy(pTarget, pPackage, size);
    pTarget[(rand() % (size + 1))] = 255;
}

void vHammingAdd_2(uint8_t *pcBlock, uint8_t *hamm)
{
  *(hamm) = 0;
  *(hamm + 1) = 0;
  *(hamm + 2) = 0;
  *(hamm + 3) = 0;
  *(hamm + 4) = 0;
    uint8_t *ui8HammArr[21] =
    {&hamm[0],
    &hamm[1],
    &pcBlock[0],
    &hamm[2],
    &pcBlock[1],
    &pcBlock[2],
    &pcBlock[3],
    &hamm[3],
    &pcBlock[4],
    &pcBlock[5],
    &pcBlock[6],
    &pcBlock[7],
    &pcBlock[8],
    &pcBlock[9],
    &pcBlock[10],
    &hamm[4],
    &pcBlock[11],
    &pcBlock[12],
    &pcBlock[13],
    &pcBlock[14],
    &pcBlock[15]
    };
  for(uint8_t i = 0; i < 21; i = i + 2)
  {
    *ui8HammArr[0] ^= *ui8HammArr[i];
  }
  for(uint8_t i = 1; i < 21; i = i + 4)
  {
      *ui8HammArr[1] ^= *ui8HammArr[i];
      *ui8HammArr[1] ^= *ui8HammArr[i+1];
  }
  *ui8HammArr[3] ^= *ui8HammArr[4] ^
  *ui8HammArr[5] ^ *ui8HammArr[6] ^
  *ui8HammArr[11] ^ *ui8HammArr[12] ^
  *ui8HammArr[13] ^ *ui8HammArr[14] ^
  *ui8HammArr[19] ^ *ui8HammArr[20];

  for(uint8_t i = 7; i < 16; i++)
  {
    *ui8HammArr[7] ^= *ui8HammArr[i];
  }
  for(uint8_t i = 15; i < 21; i++)
  {
    *ui8HammArr[15] ^= *ui8HammArr[i];
  }
}

void vHammingCheck_2(uint8_t *pcBlock, uint8_t *hamm_received)
{
  uint8_t hamm_r[5] =
  {hamm_received[0],
  (hamm_received[1]),
  (hamm_received[2]),
  (hamm_received[3]),
  (hamm_received[4])};
  uint8_t hamm[5] = {0};
  vHammingAdd_2(pcBlock, hamm);
  uint8_t *ui8HammArr[21] =
  {&hamm[0],
  &hamm[1],
  &pcBlock[0],
  &hamm[2],
  &pcBlock[1],
  &pcBlock[2],
  &pcBlock[3],
  &hamm[3],
  &pcBlock[4],
  &pcBlock[5],
  &pcBlock[6],
  &pcBlock[7],
  &pcBlock[8],
  &pcBlock[9],
  &pcBlock[10],
  &hamm[4],
  &pcBlock[11],
  &pcBlock[12],
  &pcBlock[13],
  &pcBlock[14],
  &pcBlock[15]
  };
  uint8_t k = 22;
  for(uint8_t i = 0; i<= 7; i++)
  {
    k =
    (((hamm[0] ^ hamm_r[0]) & (1 << i))? 1  : 0) +
    (((hamm[1] ^ hamm_r[1]) & (1 << i))? 2  : 0) +
    (((hamm[2] ^ hamm_r[2]) & (1 << i))? 4  : 0) +
    (((hamm[3] ^ hamm_r[3]) & (1 << i))? 8  : 0) +
    (((hamm[4] ^ hamm_r[4]) & (1 << i))? 16 : 0);
    if((k <= 21) && (k!=0))
    {
//        *ui8HammArr[k-1] = ((*ui8HammArr[k-1]) & (1 << i))?((*ui8HammArr[k-1]) ^ (0xFF & (1 << i))):((*ui8HammArr[k-1]) ^ (1 << i));
        *ui8HammArr[k-1] = ((*ui8HammArr[k-1]) & (1 << i))?((*ui8HammArr[k-1]) ^ (1 << i)):((*ui8HammArr[k-1]) | (1 << i));
    }
  }
  for(uint8_t i = 0; i <5; i++)
  {
    *(hamm_received + i) = hamm[i];
  }
}
