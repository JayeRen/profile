using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MonsterMotor : MonoBehaviour
{

    private CharacterController controller;

    // orginal distance between the monster and the player
    private PlayerMotor playerMotor;
    private PlayerHealth playerHealth;
    // attack time gap of the monster
    private Transform playerTransform;
    private float offset;
    private Vector3 movement;
    private float verticalSpeed;
    private float attackGap = 3.0f;
    private float dashSpeed = 2.0f;
    private float jumpHeight = 20.0f;
    private float gravity = 0.4f;
    private float countdown;



    void Start()
    {
        controller = GetComponent<CharacterController>();
        playerMotor = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerMotor>();
        playerHealth = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerHealth>();
        playerTransform = GameObject.FindGameObjectWithTag("Player").GetComponent<Transform>();
        offset = playerTransform.position.z - transform.position.z;
        countdown = attackGap;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        movement = Vector3.zero;
        // forward speed
        movement.z = playerMotor.forwardSpeed;

        // attack every [attackGap] seconds
        countdown -= Time.deltaTime;
        if (countdown <= 0)
        {
            movement.z += dashSpeed;
            verticalSpeed = jumpHeight;
            countdown = attackGap;
        }
        // jumping
        else if (!controller.isGrounded && transform.position.y > 3.0f)
        {
            movement.z += dashSpeed;
            verticalSpeed -= gravity;
        }
        else if ((playerTransform.position.z - transform.position.z) <= offset)
        {
            movement.z -= dashSpeed;
        }

        movement.y = verticalSpeed;

        controller.Move(movement * Time.deltaTime);
        // keep the monster from falling into holes
        if (transform.position.y <= 3.0f)
        {
            Vector3 onGround = Vector3.zero;
            onGround.x = transform.position.x;
            onGround.z = transform.position.z;
            onGround.y = 3.0f;
            transform.position = onGround;
        }
    }

    void OnTriggerEnter(Collider other)
    {
        // distroy any obstacles along the way
        if (other.gameObject.CompareTag("Hand"))
        {
            other.gameObject.SetActive(false);
        }
        // player got caught
        if (other.gameObject.CompareTag("Player"))
        {
            playerHealth.takeDamage(100);
        }
    }
    // jump forward [distance] amount of distance
    Vector3 jump(float distance)
    {
        return Vector3.forward * distance;
    }
}
