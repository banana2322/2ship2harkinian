#include <libultraship/bridge.h>
#include "2s2h/GameInteractor/GameInteractor.h"

extern "C" {
#include "z64.h"
#include <variables.h>
extern PlayState* gPlayState;
extern SaveContext gSaveContext;
void Flags_SetWeekEventReg(s32 flag);
}

void RegisterSkipIntroSequence() {
    REGISTER_VB_SHOULD(VB_PLAY_TRANSITION_CS, {
        // Intro cutscene
        if (!(gSaveContext.save.entrance == ENTRANCE(CUTSCENE, 0) && gSaveContext.save.cutsceneIndex == 0) ||
            gSaveContext.save.isFirstCycle) {
            return;
        }

        if (CVarGetInteger("gEnhancements.Cutscenes.SkipIntroSequence", 0)) {
            gSaveContext.save.entrance = ENTRANCE(SOUTH_CLOCK_TOWN, 0);
            gSaveContext.save.hasTatl = true;
            gSaveContext.cycleSceneFlags[SCENE_INSIDETOWER].switch0 |= (1 << 0);
            gSaveContext.cycleSceneFlags[SCENE_OPENINGDAN].switch0 |= (1 << 2);
            gSaveContext.cycleSceneFlags[SCENE_OPENINGDAN].switch0 |= (1 << 0);
            gSaveContext.save.playerForm = PLAYER_FORM_DEKU;

            // Marks when Link enters clock town for the first time.
            // This should be true forever, it is not "just" for "first cycle". Don't unset it in the first cycle skip.
            gSaveContext.save.isFirstCycle = true;

            // Mark chest as opened and give the player the Deku Nuts
            gSaveContext.cycleSceneFlags[SCENE_OPENINGDAN].chest |= (1 << 0);
            Item_Give(gPlayState, ITEM_DEKU_NUTS_10);

            if (CVarGetInteger("gEnhancements.Cutscenes.SkipFirstCycle", 0)) {
                gSaveContext.save.playerForm = PLAYER_FORM_HUMAN;
                gSaveContext.save.saveInfo.playerData.isMagicAcquired = true;
                Item_Give(gPlayState, ITEM_OCARINA_OF_TIME);
                Item_Give(gPlayState, ITEM_MASK_DEKU);
                gSaveContext.save.saveInfo.inventory.questItems |= (1 << QUEST_SONG_TIME) | (1 << QUEST_SONG_HEALING);
                gSaveContext.save.saveInfo.playerData.threeDayResetCount = 1;

                // Lose nuts
                AMMO(ITEM_DEKU_NUT) = 0;

                // Tatl's text at seeing the broken great fairy
                gSaveContext.cycleSceneFlags[SCENE_YOUSEI_IZUMI].switch0 |= (1 << 10);

                // Tatl's text when first entering South Clock Town
                SET_WEEKEVENTREG(WEEKEVENTREG_59_04);

                // Set Tatl second cycle (?)
                SET_WEEKEVENTREG(WEEKEVENTREG_31_04);

                // Set Entrance Cutscene flags for Clock Town
                SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_EAST_CLOCK_TOWN);
                SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WEST_CLOCK_TOWN);
                SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_NORTH_CLOCK_TOWN);
            }
        }
    });
}
