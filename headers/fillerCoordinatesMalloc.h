/*
    Arkanoid is a classic arcade-style game where the objective is to complete levels by destroying targets with a bouncing ball.
    Copyright (C) 2024  Deezer13

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

unsigned short **fillerCoordinatesMalloc(unsigned short *sizeFillerCoordinates)
{
    unsigned short **pointer = (unsigned short **)malloc(sizeof(unsigned short *) * (*sizeFillerCoordinates));
    if (!pointer)
    {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    unsigned short *block = (unsigned short *)malloc(sizeof(unsigned short) * 2 * (*sizeFillerCoordinates));
    if (!block)
    {
        perror("Unable to allocate memory");
        free(pointer);
        exit(EXIT_FAILURE);
    }

    for (unsigned short i = 0; i < *sizeFillerCoordinates; i++)
        pointer[i] = &block[i * 2];

    return pointer;
}
