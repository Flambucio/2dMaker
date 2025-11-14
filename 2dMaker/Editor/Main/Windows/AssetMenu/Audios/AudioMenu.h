#pragma once
#include "AudiosAdd.h"
#include "AudiosDelete.h"

namespace D2Maker
{
    namespace GUI
    {
        class AudioMenu
        {
        private:
            std::vector<std::string> audioNames;
            GUIAPI::Menu menu;
            GUIAPI::ButtonWithCallback<> deleteBtn;
            GUIAPI::ButtonWithCallback<> addBtn;
            AudioAdding aAdd;
            AudioDelete aDel;

            void DeleteCallback();

        public:
            D2Maker::PreviewTextureWindow previewWindow;

            AudioMenu();

            void LoadAudioNames();
            void Update();
        };
    }
}
