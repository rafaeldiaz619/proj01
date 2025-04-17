//////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2020 Prof. AJ Bieszczad. All rights reserved.
///
//////////////////////////////////////////////////////////////////////////
/*
 * Rafael Diaz
 * Spring 2025
 * COMP 362 Section 1 - Operating Systems
 */

#include "disk.h"

disk_t disk;

/***
 *
 * Verifies correctness of the logical block address, and then translates it to a cylinder-head-sector equivalent.
 *
 */
CIDEV_RET_CODE lba2chs(lba_t lba, chs_t *chs)
{
    if (lba >= MAX_LOGICAL_BLOCK)
        return CIDEV_ADDRESS_ERROR;
// todone: implement

    unsigned int blocks_per_cyl = NUM_OF_HEADS * NUM_OF_SECTS;
    chs->cyl = lba / blocks_per_cyl;
    unsigned int rem = lba % blocks_per_cyl;
    chs->head = rem / NUM_OF_SECTS;
    chs->sect = rem % NUM_OF_SECTS;
    return CIDEV_SUCCESS;
}

/***
 *
 * Verifies correctness of the cylinder-head-sector address, and then translates it to a logical block address.
 *
 */
CIDEV_RET_CODE chs2lba(chs_t *chs, lba_t *lba)
{
// todone: implement
    if (chs->cyl >= NUM_OF_CYLS || chs->head >= NUM_OF_HEADS || chs->sect >= NUM_OF_SECTS)
        return CIDEV_ADDRESS_ERROR;

    *lba = (lba_t)chs->cyl * NUM_OF_HEADS * NUM_OF_SECTS
         + (lba_t)chs->head * NUM_OF_SECTS
         + (lba_t)chs->sect;
    return CIDEV_SUCCESS;
}
/***
 *
 * Verifies the parameters, then allocates space for the requested data (using
 * the caller-provided pointer buffer), copies the data from the disk to the buffer,
 * and appends '\0' to terminate the string.
 *
simulate: break buffer up into sector size chunks and write/read each one into their own. Logical block address.
read/write 3 full sectors into 3 consequtive LBAs plus 1 left over sector into the LBA after the other
buffer,size
array of, size characters to read/write to the disk
in read: memcpy ((*buffer) + offset, disk [cyl][head][sect], SECT_SIZE) how far into buffer do we copy the sectpr
LBA address -> 0, 0, 0 CHS disk [0][0][0] is a 16 byte array
1 byte is a character

if you have size = 54
how many sectors?
4 sectors?

if lba = 128
which lbas will be written to?
129 130 131 132.. to 54 if using size 54

what will the values of offset be during each write?
4?

 */
CIDEV_RET_CODE readDisk(lba_t lba, unsigned int size, char **buffer)
{
// todone: verify parameters
    if (lba >= MAX_LOGICAL_BLOCK || buffer == NULL)
        return CIDEV_ADDRESS_ERROR;

    *buffer = (char *)malloc(size + 1); // todone: modify as required
    if (*buffer == NULL)
        return CIDEV_SPACE_ERROR;

    unsigned int bytes_left = size;
    unsigned int offset = 0;
    lba_t current = lba;
    chs_t chs;


    while (bytes_left > 0) {
        if (current >= MAX_LOGICAL_BLOCK) {
            free(*buffer);
            return CIDEV_ADDRESS_ERROR;
        }

        lba2chs(current, &chs);
        unsigned int chunk = (bytes_left < SECT_SIZE ? bytes_left : SECT_SIZE);
        memcpy(*buffer + offset, disk[chs.cyl][chs.head][chs.sect], chunk);

        bytes_left -= chunk;
        offset += chunk;
        current++;
    }

    // todone: implement
    (*buffer)[size] = '\0';
    return CIDEV_SUCCESS;

}

/***
 *
 * An auxiliary function to fill a single disk block with '.'
 *
*/
CIDEV_RET_CODE clearBlock(lba_t lba)
{
    char *writeBuffer;
#ifdef __DEBUG_DISK
    writeBuffer = malloc(SECT_SIZE);
#else
    writeBuffer = kmalloc(SECT_SIZE, GFP_USER);
#endif
    if (writeBuffer == NULL)
        return CIDEV_SPACE_ERROR;

    for (int i = 0; i < SECT_SIZE; i++)
        writeBuffer[i] = '.';

    CIDEV_RET_CODE errCode = writeDisk(lba, writeBuffer);

#ifdef __DEBUG_DISK
    free(writeBuffer);
#else
    kfree(writeBuffer);
#endif

    return errCode;
}

/***
 *
 * Validates the parameters, and then writes the caller-provided data to the disk starting at the block pointed
 * to by the logical block address.
 *
 */
CIDEV_RET_CODE writeDisk(lba_t lba, char *buffer)
{
// todone: verify the parameters
    if (lba >= MAX_LOGICAL_BLOCK || buffer == NULL)
        return CIDEV_ADDRESS_ERROR;

    unsigned int size = (unsigned int)strlen(buffer);
    unsigned int bytes_left = size;
    unsigned int offset = 0;
    lba_t current = lba;
    chs_t chs;

    while (bytes_left > 0) {
        if (current >= MAX_LOGICAL_BLOCK)
            return CIDEV_ADDRESS_ERROR;

        lba2chs(current, &chs);
        unsigned int chunk = (bytes_left < SECT_SIZE ? bytes_left : SECT_SIZE);

        memcpy(disk[chs.cyl][chs.head][chs.sect], buffer + offset, chunk);

        if (chunk < SECT_SIZE) {
            for (unsigned int j = chunk; j < SECT_SIZE; j++)
                disk[chs.cyl][chs.head][chs.sect][j] = '.';
        }

        bytes_left -= chunk;
        offset += chunk;
        current++;
    }

    return CIDEV_SUCCESS;
}
