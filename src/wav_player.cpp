/**
 * @brief A simple wav player that uses SDL2
 * 
 */

#include <SDL2/SDL.h>
#include <iostream>

void cleanAndExit(bool sdl_quit, int exit_code)
{
    if (sdl_quit)
        SDL_Quit();
    exit(exit_code);
}

int main()
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        std::cerr << SDL_GetError() << '\n';
        return 1;
    }

    SDL_AudioSpec wav_spec;
    Uint8*        wav_buffer = nullptr;
    Uint32        wav_length = 0;

    const char* file_name = "waveform.wav";
    if (!SDL_LoadWAV(file_name, &wav_spec, &wav_buffer, &wav_length))
    {
        std::cerr << SDL_GetError() << '\n';
        cleanAndExit(true, 1);
    }

    // Open an audio device using the WAV's format.
    SDL_AudioDeviceID device =
        SDL_OpenAudioDevice(nullptr, 0, &wav_spec, nullptr, 0);

    if (device == 0)
    {
        std::cerr << SDL_GetError() << '\n';
        SDL_FreeWAV(wav_buffer);
        cleanAndExit(true, 1);
    }

    // Submit the audio data to the device.
    if (SDL_QueueAudio(device, wav_buffer, wav_length) < 0)
        std::cerr << SDL_GetError() << '\n';

    // Start playback.
    SDL_PauseAudioDevice(device, 0);

    // Wait until all queued audio has been played.
    while (SDL_GetQueuedAudioSize(device) > 0)
        SDL_Delay(10);

    // Cleanup.
    SDL_CloseAudioDevice(device);
    SDL_FreeWAV(wav_buffer);

    cleanAndExit(true, 0);
}
