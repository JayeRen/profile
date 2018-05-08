using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMotor : MonoBehaviour
{
    private Transform playerTransform;
    private Vector3 offset;

    // Use this for initialization
    void Start()
    {
        playerTransform = GameObject.FindGameObjectWithTag("Player").transform;
        offset = transform.position - playerTransform.position;

    }

    // Update is called once per frame
    void Update()
    {
        Vector3 current = playerTransform.position + offset;
        current.x = 0;
        transform.position = current;
    }
}
