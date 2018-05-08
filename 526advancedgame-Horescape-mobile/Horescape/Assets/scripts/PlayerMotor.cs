using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerMotor : MonoBehaviour
{

    private CharacterController controller;
    private Vector3 movement;
    public float forwardSpeed = 5.0f;
    // accelerated speed
    private float accelerated = 5.0f;
    // countdown for accelerating
    private float countdown;
    private float verticalSpeed;
    private float gravity = 0.4f;
	private bool jumpIndicator = false;
    private float jumpHeight = 9.0f;
    // Use this for initialization

	// ios touch
	private Vector3 touchOrigin = Vector3.zero;

    PlayerHealth playerHealth;
    void Start()
    {
        controller = GetComponent<CharacterController>();
        playerHealth = GetComponent<PlayerHealth>();
        countdown = accelerated;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        movement = Vector3.zero;
        // speed up every [accelerated] seconds
        countdown -= Time.deltaTime;
        if (countdown <= 0)
        {
            forwardSpeed += 0.2f;
            countdown = accelerated;
        }
        movement.z = forwardSpeed;
        // left & right movement from input
        //movement.x = Input.GetAxisRaw("Horizontal") * -5.0f;

		// gravity
		if (controller.isGrounded)
		{
			if (Input.GetButton("Jump"))
			{
				verticalSpeed = jumpHeight;
			}
			else
			{
				// a small force to keep the player on the ground
				verticalSpeed = -0.1f;
			}
		}
		else
		{
			verticalSpeed -= gravity;
		}

        if (Input.touchCount > 0)
        {
            Touch myTouch = Input.GetTouch(0);

			TouchPhase phase = myTouch.phase;
			if (phase == TouchPhase.Began) {
				//If so, set touchOrigin to the position of that touch
				touchOrigin = myTouch.position;
			} else if (phase == TouchPhase.Moved) {
				jumpIndicator = true;
			} else if (phase == TouchPhase.Ended && jumpIndicator == true) {
				jumpIndicator = false;
				if (myTouch.position.y > touchOrigin.y && controller.isGrounded)
					verticalSpeed = jumpHeight;
			} else if (phase == TouchPhase.Ended || phase == TouchPhase.Stationary) {
				if (myTouch.position.y < Screen.height / 2) {
					if (myTouch.position.x > Screen.width / 2)
					{
						movement.x = -3.0f;
					}
					else
					{
						movement.x = 3.0f;
					}
				}		
			}
        }

        movement.y = verticalSpeed;

        controller.Move(movement * Time.deltaTime);

        // falling into the fall =death
        if (controller.transform.position.y < 0)
        {
            playerHealth.takeDamage(100);
        }

    }

    void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("Hand"))
        {
            other.gameObject.SetActive(false);
            playerHealth.takeDamage(40);
        }
    }
}
