#include "hash_table.h"

hash_table::hash_table()
{
    hash_count = 0;
    hash_hmax = 6;
    hash_vmax = 20;
    hash_grow_factor = 3;
    hash_loadfactor = 0;
    table = new linked_list[hash_hmax]();
}

hash_table::~hash_table()
{
    table->~linked_list();
}

linked_list *hash_table::rehash()
{
    uint32_t hash;
    uint32_t old_hmax = hash_hmax;
    hash_hmax *=  hash_grow_factor;
    switch(hash_vmax)
    {
    case 20:
    case 18:
    case 16:
    case 14:
        hash_vmax -= 2;
        break;
    case 12:
    case 11:
    case 10:
    case 9:
        hash_vmax--;
        break;
    default:
        break;
    }
    linked_list *lista = new linked_list[hash_hmax]();
    uint32_t i;
    uint8_t j;
    string temp;
    for (i = 0; i < old_hmax; i++)
    {
        int h = table[i].tam();
        for (j = 0; j < h; j++)
        {
            temp.assign(table[i].decapitate());
            hash = zhash(temp) % hash_hmax;
            lista[hash].add(temp);
        }
    }
    return lista;
}

bool hash_table::add(string input)
{
    uint32_t hash = zhash(input) % hash_hmax;
    if (table[hash].add(input))
    {
        hash_count++;
        hash_loadfactor = hash_count / hash_hmax;
        if (hash_loadfactor >= hash_vmax)
        {
            linked_list *nova = rehash();
            table->~linked_list();
            table = nova;
        }
        return true;
    }
    return false;
}

uint8_t hash_table::loadfactor()
{
    return hash_loadfactor;
}

uint64_t hash_table::total()
{
    return this->hash_count;
}

bool hash_table::del(string input)
{
    uint32_t hash = zhash(input) % hash_hmax;
    if (table[hash].del(input))
    {
        hash_count--;
        return true;
    }
    return false;
}

bool hash_table::locate(string input)
{
    uint32_t hash = zhash(input) % hash_hmax;
    if (table[hash].locate(input))
        return true;
    else
        return false;
}

uint32_t hash_table::zhash(string in) {
    uint32_t len = in.length();
    const char* key = in.c_str();
    uint32_t hash = 0x2014aed2;
    const int nblocks = len / 4;
    int i=0;

            //k é armazenado em EAX, hash em %0
    __asm__ ( " CMPL $0, %[NBLOCKS] \n\t " //comapara o valor de nblocks com 0
            " JZ 2f \n\t "         //se for zero, da um jump para a label 2 forward
            " 1: \n\t "
//            " MOVL %[I], %%eax \n\t " //pega o valor de I
//            " SALL $2, %%eax \n\t "   //multiplica por 4
//            " ADDL %[KEY], %%eax \n\t "   //soma com o endereço de blocks (local em que esta o k)
//            " MOVL (%%eax), %%eax \n\t " //armazena k em EAX
//            " IMULL $-862048943, %%eax, %%eax \n\t " // k *= c1
            " MOVL %[I], %%eax \n\t " //pega o valor de I
            " MOVL %[KEY], %%ebx \n\t " //pega o endereço de blocks
            " IMULL $-862048943, (%%ebx,%%eax,4), %%eax \n\t " // k *= c1 -> (c1 * [ebx+eax*4]) [] -> endereço onde está k
            // k = (k << r1) | (k >> (32 - r1))
//          " MOVL %%eax, %%ebx \n\t " //copia k para ebx
//          " SHLL $15, %%eax \n\t " //(k << r1)
//          " SHRL $17, %%ebx \n\t " //(k >> (32 - r1))
//          " ORL  %%ebx, %%eax \n\t "
            " ROR $17, %%eax \n\t " //faz o mesmo que as 4 linhas acima
            // k *= c2
            " IMULL $461845907, %%eax, %%eax \n\t "
            " XOR %%eax, %[HASH] \n\t " //hash ^= k;
            //hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
//          " MOVL %%edx, %%ebx \n\t " // copia hash para ebx
//          " SHLL $13, %%edx \n\t " //(hash << r2)
//          " SHRL $19, %%ebx \n\t " //(hash >> (32 - r2))
//          " ORL  %%ebx, %%edx \n\t "
            " ROR $19, %[HASH] \n\t " //faz o mesmo que as 4 linhas acima
//          " IMULL $0, %%edx, %%edx \n\t " // * m + n (usei opção abaixo por ser mais rápida)
//          " ADDL $-430675100, %0 \n\t " // + n
            " LEAL -430675100(%[HASH], %[HASH], 4), %[HASH] \n\t "
            " INCL %[I] \n\t " //incrementa i
            " MOVL %[I], %%eax \n\t " //move o valor de i para um registrador, pelo menos um dos parametros de CMP precisa ser assim
            " CMPL %[NBLOCKS], %%eax \n\t " //compara i com nblocks
            " JB 1b \n\t "         //se i < nblocks, pula para 1 backward
            " 2: \n\t "
//            " MOVL %[KEY], %%eax \n\t "
//            " MOVL %[NBLOCKS], %%eax \n\t " //pega o valor de nblocks
//            " SALL $2, %%eax \n\t "   //multiplica por 4
//            " ADDL %[KEY], %%eax \n\t "   //soma com o endereço de blocks (local em que esta o k)
//            " MOVL (%%eax), %%eax \n\t " //armazena k em EAX
            " MOVL %[LEN], %%eax \n\t " //%[LEN] = len
            " ANDL $3, %%eax \n\t " // len & 3
            " CMPL $3, %%eax \n\t " //se %[LEN] = 3
            " JE 3f \n\t "
            " CMPL $2, %%eax \n\t " //se %[LEN] = 2
            " JE 4f \n\t "
            " CMPL $1, %%eax \n\t " //se %[LEN] = 1
            " JE 5f \n\t "
            " JMP 6f \n\t "
            " 5: \n\t "
            " MOVL %[NBLOCKS], %%eax \n\t " //pega o valor de nblocks
            " MOVL %[KEY], %%ebx \n\t " //pega o endereço de blocks
            //" LEAL (%%ebx,%%eax,4), %%eax \n\t " // k = [blocks+nblocks*4]
            " MOVL (%%ebx,%%eax,4), %%eax \n\t "
            " SHLL $24,%%eax \n\t "
            " SHRL $24,%%eax \n\t "
            " JMP 7f \n\t "

            " 4: \n\t "
            " MOVL %[NBLOCKS], %%eax \n\t " //pega o valor de nblocks
            " MOVL %[KEY], %%ebx \n\t " //pega o endereço de blocks
            //" LEAL (%%ebx,%%eax,4), %%eax \n\t " // k = [blocks+nblocks*4]
            " MOVL (%%ebx,%%eax,4), %%eax \n\t "
            " SHLL $16,%%eax \n\t "
            " SHRL $16,%%eax \n\t "
//            " SHLL $8,%%eax \n\t "
            " JMP 7f \n\t "

            " 3: \n\t "
            " MOVL %[NBLOCKS], %%eax \n\t " //pega o valor de nblocks
            " MOVL %[KEY], %%ebx \n\t " //pega o endereço de blocks
//            " LEAL (%%ebx,%%eax,4), %%eax \n\t " // k = [blocks+nblocks*4]
            " MOVL (%%ebx,%%eax,4), %%eax \n\t "
            " SHLL $8,%%eax \n\t "
            " SHRL $8,%%eax \n\t "
//            " SHLL $16,%%eax \n\t "

            " 7: \n\t "

            " IMULL $0xcc9e2d51, %%eax, %%eax \n\t " // k *= c1
            // k1 = (k << r1) | (k >> (32 - r1))
//            " MOVL %%eax, %%ebx \n\t " //copia k para ebx
//            " SHLL $15, %%eax \n\t " //(k << r1)
//            " SHRL $17, %%ebx \n\t " //(k >> (32 - r1))
//            " ORL  %%ebx, %%eax \n\t "
            " ROR $17, %%eax \n\t " //faz o mesmo que as 4 linhas acima
            // k *= c2
            " IMULL $461845907, %%eax, %%eax \n\t "
            " XOR %%eax, %[HASH] \n\t " //hash ^= k1;
            " 6: \n\t "
            " XORL %[LEN], %[HASH] \n\t " //hash ^= len;
            //hash ^= (hash >> 16);
            " MOVL %[HASH], %%ebx \n\t "
            " SHRL $16, %%ebx \n\t "
            " XOR %%ebx, %[HASH] \n\t "
            //hash *= -2048144789;
            " IMULL $-2048144789, %[HASH], %[HASH] \n\t "
            //hash ^= (hash >> 13);
            " MOVL %[HASH], %%ebx \n\t "
            " SHRL $13, %%ebx \n\t "
            " XOR %%ebx, %[HASH] \n\t "
            //hash *= -1028477387;
            " IMULL $-1028477387, %[HASH], %[HASH] \n\t "
            //hash ^= (hash >> 16);
            " MOVL %[HASH], %%ebx \n\t "
            " SHRL $16, %%ebx \n\t "
            " XOR %%ebx, %[HASH] \n\t "
            : [HASH] "=g"(hash)
            : "0"(hash), [I] "m"(i), [KEY] "m"(key), [NBLOCKS] "m"(nblocks), [LEN] "m"(len)
            : "ebx", "eax", "memory", "cc"
            );

    return hash;
}

//uint32_t hash_table::murmur(string in)
//{
//   	static const uint32_t c1 = 0xcc9e2d51;
//	static const uint32_t c2 = 0x1b873593;
//	static const uint32_t r1 = 15;
//	static const uint32_t r2 = 13;
//	static const uint32_t m = 5;
//	static const uint32_t n = 0xe6546b64;
//	static const uint32_t seed = 0x2014aed2;
//	const char *key;
//
//	uint32_t hash = seed;
//    uint32_t len = in.length();
//    key = in.c_str();
//	const int nblocks = len / 4;
//	const uint32_t *blocks = (const uint32_t *) key;
//	int i;
//	for (i = 0; i < nblocks; i++) {
//		uint32_t k = blocks[i];
//		k *= c1;
//		k = (k << r1) | (k >> (32 - r1));
//		k *= c2;
//
//		hash ^= k;
//		hash = ((hash << r2) | (hash >> (32 - r2))) * m + n;
//	}
//
//	const uint8_t *tail = (const uint8_t *) (key + nblocks * 4);
//	uint32_t k1 = 0;
//
//	switch (len & 3) {
//	case 3:
//		k1 ^= tail[2] << 16;
//	case 2:
//		k1 ^= tail[1] << 8;
//	case 1:
//		k1 ^= tail[0];
//
//		k1 *= c1;
//		k1 = (k1 << r1) | (k1 >> (32 - r1));
//		k1 *= c2;
//		hash ^= k1;
//	}
//
//	hash ^= len;
//	hash ^= (hash >> 16);
//	hash *= 0x85ebca6b;
//	hash ^= (hash >> 13);
//	hash *= 0xc2b2ae35;
//	hash ^= (hash >> 16);
//
//	return hash;
//
//}
//
//bool hash_table::test(string input)
//{
//    if (zhash(input) != murmur(input))
//    {
//        cout << "Fail..." << endl;
//        exit(-1);
//    }
//    return true;
//}
