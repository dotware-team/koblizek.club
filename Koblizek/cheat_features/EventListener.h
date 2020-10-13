#pragma once

#include "..\singleton.h"
#include "..\cheat_sdk\GameEvent.h"

class EventManager : GameEventListener, public Singleton<EventManager> {
public:
	EventManager();
	~EventManager();
	inline void init() {};
	void fireGameEvent(GameEvent* event_) override;
private:
	void bullet_impact(GameEvent* event_);
	void player_footstep(GameEvent * event_);
	void player_jump(GameEvent * event_);
	void player_hurt(GameEvent * event_);
	void player_purchase(GameEvent * event_);
	void player_bulletimpact(GameEvent * event_);
};