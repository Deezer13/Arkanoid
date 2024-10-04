#include "headers/irrKlang/irrKlang.h"
#include "headers/irrKlangWrapper.h"

void *createSoundEngine()
{
    return (void *)irrklang::createIrrKlangDevice();
}

void playSound(void *engine, const char *filename, int loop)
{
    if (engine)
        ((irrklang::ISoundEngine *)engine)->play2D(filename, loop);
}
