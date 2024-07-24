#pragma once



struct RigidBody
{
	glm::vec3 velocity = glm::vec3(5.0f,0.0f,0.0f);

	void drag() 
	{
		velocity -= velocity * 0.2f;
	}


	void AddForce(glm::vec3 direction) 
	{
		velocity += direction;
	}


	glm::vec3 getVelocity() 
	{
		return velocity;
	}
};

class PlayerControlled
{
public:
	RigidBody rigid;
	glm::vec3 n_position = glm::vec3(0.f, -1.8f, -1.f);
	glm::vec3 n_rotation = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 n_scale = glm::vec3(.05f, .05f, .05f);

private:
	Transform m_transform;
	EngineMesh * ship;

public:
	EngineMesh* getMesh() 
	{
		return ship;
	}
	PlayerControlled()
	{
		rigid = RigidBody();
		m_transform = Transform();
		m_transform.position = glm::vec3(1, 1, 1); 
		m_transform.rotation = glm::vec3(1, 1, 1);
		m_transform.scale = glm::vec3(1, 1, 1);

		n_position = glm::vec3(0.f, -1.8f, -1.f);
		n_rotation = glm::vec3(0.f, 0.f, 0.f);
		n_scale = glm::vec3(.05f, .05f, .05f);
	}

	PlayerControlled(unsigned int shader, EngineMesh * t_ship)
	{
		ship = t_ship;
		m_transform = Transform();
		m_transform.position = glm::vec3(1, 1, 1);
		m_transform.rotation = glm::vec3(1, 1, 1);
		m_transform.scale = glm::vec3(1, 1, 1);

		n_position = glm::vec3(0.f, -1.8f, -1.f);
		n_rotation = glm::vec3(0.f, 0.f, 0.f);
		n_scale = glm::vec3(.05f, .05f, .05f);
	}

	void Update() 
	{
		rigid.drag();
		m_transform.position += rigid.getVelocity();
		ship->n_position += n_position;
		ship->n_rotation += n_rotation;
		ship->n_scale += n_scale;
		ship->getTransform()->position += n_position;
		ship->getTransform()->rotation += n_rotation;
		ship->getTransform()->scale += n_scale;

	}


	Transform * getTrans() 
	{
		return &m_transform;
	}



};