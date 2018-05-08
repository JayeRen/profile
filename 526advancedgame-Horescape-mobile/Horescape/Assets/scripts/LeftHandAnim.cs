using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LeftHandAnim : MonoBehaviour
{
    private Vector3 teleportPoint;
    private Transform playerTransform;
    // distance between the left hand and the player
    private float distance;
    // display the animation when the left hand is within the display zone of the player
    private float displayZone = 8.0f;
    // the length of the hand that will be on the screen
    private float displayLength = 2.5f;
    private float movingSpeed = 8.0f;
    void Start()
    {
        playerTransform = GameObject.FindGameObjectWithTag("Player").transform;
    }
    void FixedUpdate()
    {
        distance = transform.position.z - playerTransform.position.z;
        // within the display zone
        if (distance <= displayZone)
        {
            // stop moving after the whole hand on the screen
            if (transform.position.x >= displayLength)
            {
                transform.position += Vector3.left * Time.deltaTime * movingSpeed;
            }
        }
    }
}
