﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


/**
 * when you modify this, please note that this information can be saved with instances
 * also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list
 **/

// Trace against Actors/Components which provide interactions.
#define DoubleHeroes_TraceChannel_Interaction					ECC_GameTraceChannel1

// Trace used by weapons, will hit physics assets instead of capsules
#define DoubleHeroes_TraceChannel_Weapon						ECC_GameTraceChannel2

// Trace used by by weapons, will hit pawn capsules instead of physics assets
#define DoubleHeroes_TraceChannel_Weapon_Capsule				ECC_GameTraceChannel3

// Trace used by by weapons, will trace through multiple pawns rather than stopping on the first hit
#define DoubleHeroes_TraceChannel_Weapon_Multi					ECC_GameTraceChannel4

// Allocated to aim assist by the ShooterCore game feature
// ECC_GameTraceChannel5
