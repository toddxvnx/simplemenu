#include <config.h>
#include <definitions.h>
#include <globals.h>
#include <logic.h>
#include <screen.h>
#include <stdlib.h>
#include <string.h>
#include <system_logic.h>
#include <SDL/SDL_events.h>

int returnTo=0;

void advanceSection() {
	currentSectionNumber++;
	setupDecorations();
	totalPages=0;
	loadGameList();
}

void rewindSection() {
	currentSectionNumber--;
	setupDecorations();
	totalPages=0;
	loadGameList();
}

void launchGame() {
	char fileToBeExecutedwithFullPath[200];
	if (favoritesSectionSelected && favoritesSize > 0) {
		strcpy(fileToBeExecutedwithFullPath,CURRENT_FAVORITE.filesDirectory);
		strcat(fileToBeExecutedwithFullPath,CURRENT_GAME_NAME);
		executeCommand(CURRENT_FAVORITE.emulatorFolder,CURRENT_FAVORITE.executable,fileToBeExecutedwithFullPath);
	} else if (CURRENT_GAME_NAME!=NULL) {
		strcpy(fileToBeExecutedwithFullPath,CURRENT_SECTION.filesDirectory);
		strcat(fileToBeExecutedwithFullPath,CURRENT_GAME_NAME);
		executeCommand(CURRENT_SECTION.emulatorFolder, CURRENT_SECTION.executable,fileToBeExecutedwithFullPath);
	}
}

void scrollDown() {
	if(CURRENT_SECTION.currentGame == gamesInPage-1) {
		if (CURRENT_SECTION.currentPage < totalPages) {
			CURRENT_SECTION.currentGame=0;
			CURRENT_SECTION.currentPage++;
			return;
		}
	}
	if (CURRENT_SECTION.currentGame < gamesInPage-1) {
		CURRENT_SECTION.currentGame++;
		return;
	}
}

void scrollUp() {
	if(CURRENT_SECTION.currentGame == 0) {
		if (CURRENT_SECTION.currentPage >0) {
			CURRENT_SECTION.currentPage--;
			gamesInPage=countGamesInPage();
			CURRENT_SECTION.currentGame=gamesInPage-1;
			return;
		}
	}
	if (CURRENT_SECTION.currentGame > 0) {
		CURRENT_SECTION.currentGame--;
		return;
	}
}

void advancePage() {
	if (CURRENT_SECTION.currentPage < totalPages) {
		CURRENT_SECTION.currentGame=0;
		CURRENT_SECTION.currentPage++;
	}
}

void rewindPage() {
	if (CURRENT_SECTION.currentPage > 0) {
		CURRENT_SECTION.currentGame=0;
		CURRENT_SECTION.currentPage--;
	}
}

void showOrHideFavorites() {
	if (favoritesSectionSelected) {
		favoritesSectionSelected=0;
		currentSectionNumber=returnTo;
		setupDecorations();
		totalPages=0;
		loadGameList();
		return;
	}
	favoritesSectionSelected=1;
	returnTo=currentSectionNumber;
	currentSectionNumber=lastSection;
	setupDecorations();
	totalPages=0;
	loadFavoritesList();
}

void removeFavorite() {
	favoritesChanged=1;
	if (favoritesSize>0) {
		for (int i=CURRENT_FAVORITE_NUMBER;i<favoritesSize;i++) {
			favorites[i]=favorites[i+1];
		}
		favoritesSize--;
		scrollUp();
		setupDecorations();
		totalPages=0;
		loadFavoritesList();
	}
}

void markAsFavorite() {
	favoritesChanged=1;
	if (favoritesSize<100) {
		strcpy(favorites[favoritesSize].name, CURRENT_GAME_NAME);
		strcpy(favorites[favoritesSize].emulatorFolder,CURRENT_SECTION.emulatorFolder);
		strcpy(favorites[favoritesSize].executable,CURRENT_SECTION.executable);
		strcpy(favorites[favoritesSize].filesDirectory,CURRENT_SECTION.filesDirectory);
		favoritesSize++;
	}
}

int performAction() {
	if (keys[BTN_SELECT] && keys[BTN_START]) {
		running=0;
		return 0;
	}
	if (keys[BTN_TA] && keys[BTN_START]) {
		freeResources();
		saveLastState();
		saveFavorites();
		exit(0);
	}
	if (keys[BTN_START]) {
		if (!favoritesSectionSelected) {
			markAsFavorite();
		} else {
			removeFavorite();
		}
		return 0;
	}
	if (keys[SDLK_RCTRL]) {
		showOrHideFavorites();
		return 0;
	}
	if(keys[BTN_TB]) {
		cycleFrequencies();
		drawHeader();
		return 0;
	}
	if(keys[BTN_TA]) {
		if(currentSectionNumber<lastSection-1) {
			advanceSection();
		}
		return 0;
	}
	if(keys[BTN_B]) {
		if(currentSectionNumber!=lastSection&&currentSectionNumber>0) {
			rewindSection();
		}
		return 0;
	}
	if (keys[BTN_A]) {
		launchGame();
		return 0;
	}
	if (keys[BTN_DOWN]) {
		scrollDown();
		return 1;
	}
	if(keys[BTN_UP]) {
		scrollUp();
		return 1;
	}
	if(keys[BTN_RIGHT]) {
		advancePage();
		return 1;
	}
	if(keys[BTN_LEFT]) {
		rewindPage();
		return 1;
	}
	return 0;
}