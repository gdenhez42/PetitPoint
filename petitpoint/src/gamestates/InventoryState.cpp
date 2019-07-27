#include "InventoryState.h"

namespace pp {
    InventoryState::InventoryState(GameState* p_prev) : m_previous(p_prev)
    {
    }

    InventoryState::~InventoryState()
    {
    }
    bool InventoryState::Init(const LWindow& p_pWindow, const RessourcesRepo& p_ressourceRepo)
    {
        // todo
        return true;
    }
    std::tuple<bool,GameState*> InventoryState::Update(const Commands& p_command)
    {
        switch(p_command.GetEvent()) {
        case Commands::PP_EVENT_CANCEL:
            GameState* previous = m_previous;
            delete this;
            return std::make_tuple(true, previous);
        }

        return std::make_tuple(true, this);
    }

    void InventoryState::Render()
    {
        // render the inventory
    }
}

