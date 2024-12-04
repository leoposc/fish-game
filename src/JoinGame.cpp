#include <SDL2/SDL.h>
#include <string>

bool isValidIPv4(std::string ip) {
	int num = 0;
	int dots = 0;

	for (int i = 0; i < ip.length(); i++) {
		if (ip[i] == '.') {
			dots++;
			if (num > 255) {
				return false;
			}
			num = 0;
		} else if (ip[i] >= '0' && ip[i] <= '9') {
			num = num * 10 + (ip[i] - '0');
		} else {
			return false;
		}
	}

	if (dots != 3) {
		return false;
	}

	if (num > 255) {
		return false;
	}

	return true;
}

std::string joinGame() {
	SDL_Event event;
	SDL_Color textColor = {0, 0, 0, 0}; // black

	// The current input text.
	std::string inputText = "Enter IP Address here";
	gInputTextTexture.loadFromRenderedText(inputText.c_str(), textColor);

	SDL_StartTextInput();

	bool running = true;

	while (running) {
		bool renderText = false;

		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_BACKSPACE:
					if (inputText.length() > 0) {
						inputText.pop_back();
						renderText = true;
					}
					break;
				case SDLK_RETURN:
					// check if the input is a valid IPv4 address
					// if not, show an error message
					if (isValidIPv4(inputText)) {
						running = false;
					} else {
						inputText = "Invalid IP Address";
						gInputTextTexture.loadFromRenderedText(inputText.c_str(), textColor);
					}
					running = false;
					break;
				case SDLK_c:
					if (SDL_GetModState() & KMOD_CTRL) {
						SDL_SetClipboardText(inputText.c_str());
					}
					break;
				case SDLK_v:
					if (SDL_GetModState() & KMOD_CTRL) {
						char *tmpText = SDL_GetClipboardText();
						inputText = tmpText;
						SDL_free(tmpText);

						renderText = true;
					}
					break;
				}
				break;
			case SDL_TEXTINPUT:
				inputText += event.text.text;
				renderText = true;
				break;
			}

			if (renderText) {
				if (inputText.length() == 0) {
					inputText = " ";
				}

				gInputTextTexture.loadFromRenderedText(inputText.c_str(), textColor);
			}
		}
		SDL_SetRenderDrawColor(clientGame::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(clientGame::renderer);

		gPromptTextTexture.render((SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 0);
		gInputTextTexture.render((SCREEN_WIDTH - gInputTextTexture.getWidth()) / 2, gPromptTextTexture.getHeight());
	}

	SDL_StopTextInput();
	return inputText;
}