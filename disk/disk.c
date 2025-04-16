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

// todo: implement

//   readDisk(lba, sizeof(chs_t), (char *)&chs);


    return CIDEV_SUCCESS;
}

/***
 *
 * Verifies correctness of the cylinder-head-sector address, and then translates it to a logical block address.
 *
 */
CIDEV_RET_CODE chs2lba(chs_t *chs, lba_t *lba)
{
// todo: implement

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
// todo: verify parameters

    chs_t chs;

   *buffer = malloc(11*sizeof(char)); // todo: modify as required
    strcpy(*buffer, "   ");

    CIDEV_RET_CODE errCode = CIDEV_SUCCESS;

    // todo: implement
    memcpy((*buffer) + 16, disk [NUM_OF_CYLS][NUM_OF_HEADS][NUM_OF_SECTS], SECT_SIZE);
    return errCode;
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
 *
 */
CIDEV_RET_CODE writeDisk(lba_t lba, char *buffer)
{
// todo: verify the parameters

    CIDEV_RET_CODE errCode = CIDEV_SUCCESS;

    chs_t chs;

// todo: implement

    return errCode;
}
