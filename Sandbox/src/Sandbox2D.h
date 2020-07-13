#pragma once

#include <Renderent.h>

#include <glm/glm.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>

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
	
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(m_Position.x, m_Position.y);
		physicsObject = world.CreateBody(&bodyDef);

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox((m_Size.x * 0.99f ) / 2.0f, (m_Size.y * 0.99f) / 2.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;

		physicsObject->CreateFixture(&fixtureDef);
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

class Sandbox2D : public Renderent::Layer {

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Renderent::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Renderent::Event& e) override;

private:
	Renderent::Ref<Renderent::VertexArray> m_SquareVA;
	Renderent::OrthographicCameraController m_CameraController;
	Renderent::Ref<Renderent::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	Renderent::Ref<Renderent::Texture2D> m_Checkerboard;

	Player* m_Player;
	b2World* m_World;
	Floor* m_Floor;

	float m_CurrentTime = 0.0f;
};

