/**********************************************************************
 *  Mönsterigenkänning readme.txt
 **********************************************************************/

/**********************************************************************
 *  Empirisk    Fyll i tabellen nedan med riktiga körtider i sekunder
 *  analys      när det känns vettigt att vänta på hela beräkningen.
 *              Ge uppskattningar av körtiden i övriga fall.
 *
 **********************************************************************/
    
      N       brute       sortering
 ----------------------------------
    150		50 ms		21 ms
    200		114 ms		28 ms
    300		330 ms		65 ms
    400		764 ms		105 ms
    800		5814 ms		364 ms
   1600		46393 ms	1508 ms
   3200		375392 ms	6134 ms
   6400		~3100000 ms	25641 ms
  12800		~25000000 ms	107491 ms


/**********************************************************************
 *  Teoretisk   Ge ordo-uttryck för värstafallstiden för programmen som
 *  analys      en funktion av N. Ge en kort motivering.
 *
 **********************************************************************/

Brute: O(n⁴)

Sortering: O(n² * log(n))

Värsta fall för brute har 4 loop nästning där varje loop kör N gånger.

Värsta fall för fast har 2 loop nästning för N där den innersta loopen gör operation på O(log(n))
