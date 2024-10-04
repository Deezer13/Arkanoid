#ifdef __cplusplus
extern "C"
{
    #endif

    void *createSoundEngine();
    void playSound(void *engine, const char *filename, int loop);

    #ifdef __cplusplus
}
#endif
