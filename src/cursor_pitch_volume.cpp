#include <SDL2/SDL.h>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <iostream>
#include <numbers>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 500;

constexpr float MIN_FREQUENCY = 110.0f;
constexpr float MAX_FREQUENCY = 880.0f;

constexpr double PI = std::numbers::pi;

struct AudioState
{
    std::atomic<float> frequency{440.0f};
    std::atomic<float> volume{0.5f};

    double phase = 0.0;
    int    sampleRate = 48000;
};

// SDL calls this function whenever it needs more audio samples.
void audioCallback(void* userdata, Uint8* stream, int len)
{
    auto* audio = static_cast<AudioState*>(userdata);
    auto* output = reinterpret_cast<float*>(stream);

    const int frameCount = len / (sizeof(float) * 2);

    const float frequency = audio->frequency.load();
    const float volume = audio->volume.load();

    const double phaseIncrement = 2.0 * PI * frequency / audio->sampleRate;

    for (int i = 0; i < frameCount; ++i)
    {
        // Generate one sample of a sine wave.
        const float sample =
            static_cast<float>(std::sin(audio->phase)) * volume;

        // Stereo: write the same sample to left and right channels.
        output[i * 2] = sample;
        output[i * 2 + 1] = sample;

        // Advance the oscillator.
        audio->phase += phaseIncrement;

        if (audio->phase >= 2.0 * PI)
            audio->phase -= 2.0 * PI;
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Mouse Synthesiser",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    AudioState audio;

    SDL_AudioSpec desired{};
    desired.freq = audio.sampleRate;
    desired.format = AUDIO_F32SYS;
    desired.channels = 2;
    desired.samples = 512;
    desired.callback = audioCallback;
    desired.userdata = &audio;

    SDL_AudioDeviceID device =
        SDL_OpenAudioDevice(nullptr, 0, &desired, nullptr, 0);

    if (device == 0)
    {
        std::cerr << "Audio device opening failed: " << SDL_GetError() << '\n';

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Start audio playback.
    SDL_PauseAudioDevice(device, 0);

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Keep the coordinates inside the window.
        mouseX = std::clamp(mouseX, 0, WINDOW_WIDTH - 1);
        mouseY = std::clamp(mouseY, 0, WINDOW_HEIGHT - 1);

        // Normalise X to the range [0, 1].
        const float x = static_cast<float>(mouseX) / (WINDOW_WIDTH - 1);

        // Normalise Y to the range [0, 1].
        const float y = static_cast<float>(mouseY) / (WINDOW_HEIGHT - 1);

        // Map X to frequency. Logarithmic mapping gives
        // a more natural-feeling pitch control.
        const float frequency =
            MIN_FREQUENCY * std::pow(MAX_FREQUENCY / MIN_FREQUENCY, x);

        // Top = maximum volume; bottom = silence.
        const float volume = 1.0f - y;

        // Send the new values to the audio thread.
        audio.frequency.store(frequency);
        audio.volume.store(volume);

        // Draw the window.
        SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 80, 180, 255, 255);

        // Vertical line: current pitch.
        SDL_RenderDrawLine(renderer, mouseX, 0, mouseX, WINDOW_HEIGHT);

        // Horizontal line: current volume.
        SDL_RenderDrawLine(renderer, 0, mouseY, WINDOW_WIDTH, mouseY);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    // Stop audio before destroying the data it uses.
    SDL_PauseAudioDevice(device, 1);
    SDL_CloseAudioDevice(device);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}