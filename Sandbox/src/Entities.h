#pragma once

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_shape.h>
#include <box2d/b2_world.h>
#include <glm/glm.hpp>

#include <Renderent.h>

#include "ParticleSystem.h"


class Player {

public:
	void processInput(const Renderent::Timestep& ts) {
		float desiredVel = 0;
		if (Renderent::Input::IsKeyPressed(RE_KEY_RIGHT)) {
			desiredVel = movementSpeed;
		}
		else if (Renderent::Input::IsKeyPressed(RE_KEY_LEFT)) {
			desiredVel = -movementSpeed;
		}

		b2Vec2 vel = physicsObject->GetLinearVelocity();
		float velChange = desiredVel - vel.x;
		float impulse = physicsObject->GetMass() * velChange;
		physicsObject->ApplyLinearImpulse(b2Vec2(impulse, 0), physicsObject->GetWorldCenter(), true);
	}

	void OnUpdate(Renderent::Timestep ts, ParticleSystem& pSystem) {
		for (float y = physicsObject->GetPosition().y - m_Size.y / 2.0f; y < physicsObject->GetPosition().y + m_Size.y / 2.0f; y += 0.07f) {
			particle.Position = { physicsObject->GetPosition().x, y };
			pSystem.Emit(particle);
		}
	}

	void processEvent(Renderent::KeyPressedEvent& e) {
		if (e.GetKeyCode() == RE_KEY_SPACE && !e.GetIsRepeated()) {
			float desiredVel = 5;

			b2Vec2 vel = physicsObject->GetLinearVelocity();
			float velChange = desiredVel - vel.y;
			float impulse = physicsObject->GetMass() * velChange;
			physicsObject->ApplyLinearImpulse(b2Vec2(0, impulse), physicsObject->GetWorldCenter(), true);
		}
	}

	Player(glm::vec2 position, glm::vec2 size,
		glm::vec4 color, b2World& world)
		: m_Position(position), m_Color(color), m_Size(size) {

		// Setup Physics
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(m_Position.x, m_Position.y);
		physicsObject = world.CreateBody(&bodyDef);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox((m_Size.x * 0.99f) / 2.0f, (m_Size.y * 0.99f) / 2.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		physicsObject->CreateFixture(&fixtureDef);


		particle.Velocity = { 0.0f, 0.0f };
		particle.Position = { 0.0f, 0.0f };
		particle.VelocityVariation = { 1.0f, 2.0f };
		particle.ColorBegin = { 0.0f, 0.4f, 1.0f, 0.4f };
		particle.ColorEnd = { 1.0f, 0.4f, 1.0f, 0.4f };
		particle.SizeBegin = size.x / 10.0f;
		particle.SizeVariation = 0.03;
		particle.SizeEnd = 0.0f;
		particle.LifeTime = 1.0f;
	}

	glm::vec2 GetPosition() const { return { physicsObject->GetPosition().x, physicsObject->GetPosition().y }; }
	const glm::vec4& GetColor() const { return m_Color; }
	const glm::vec2& GetSize() const { return m_Size; }

private:
	glm::vec2 m_Position;
	glm::vec4 m_Color;
	glm::vec2 m_Size;

	const float movementSpeed = 1.0f;

	b2Body* physicsObject;
	ParticleProps particle;
};


class Floor {

public:

	Floor(glm::vec2 position, glm::vec2 size, glm::vec4 color, b2World& world)
		: m_Position(position), m_Size(size), m_Color(color) {

		b2BodyDef bodyDef;
		bodyDef.position.Set(m_Position.x, m_Position.y);

		physicsObject = world.CreateBody(&bodyDef);

		b2PolygonShape boxShape;
		boxShape.SetAsBox((size.x * 0.99f) / 2.0f, (size.y * 0.99f) / 2.0f);

		physicsObject->CreateFixture(&boxShape, 0.0f);
	}

	const glm::vec2& GetPosition() const { return m_Position; }
	const glm::vec2& GetSize() const { return m_Size; }
	const glm::vec4& GetColor() const { return m_Color; }

private:
	glm::vec2 m_Position;
	glm::vec2 m_Size;
	glm::vec4 m_Color;

	b2Body* physicsObject;
};