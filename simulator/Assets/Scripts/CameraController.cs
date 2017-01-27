using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour {

    public GameObject robot;
    private Vector3 offset;
	// Use this for initialization
	void Start () {
        offset = transform.position - robot.transform.position;
	}
	
	// Update is called once per frame
	void LateUpdate () {
        transform.position = robot.transform.position + offset;
        transform.Rotate(new Vector3(0,0,0));
	}
}
