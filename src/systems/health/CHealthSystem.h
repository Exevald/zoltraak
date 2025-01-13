#pragma once

class CHealthSystem
{
public:
	static void Init();
	static void Update();

private:
	static void OnAttackCollied(int damage, int personId);
	static void OnStrikeCreated(int damage);
};
