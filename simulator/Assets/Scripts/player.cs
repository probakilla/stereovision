using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class player : MonoBehaviour {

    public Animator anim;
    public Rigidbody rbody;

    private float inputH;
    private float inputV;

    public float m_Speed;
    public float m_TurnSpeed;
    private float m_MovementInputValue;         // The current value of the movement input.
    private float m_TurnInputValue;             // The current value of the turn input.

    private bool run;

	// Use this for initialization
	void Start () {
        anim = GetComponent<Animator>();
        rbody = GetComponent<Rigidbody>();
        run = false;

        // When the tank is turned on, make sure it's not kinematic.
        rbody.isKinematic = false;

        // Also reset the input values.
        m_MovementInputValue = 0f;
        m_TurnInputValue = 0f;
        m_Speed = 1f;
        m_TurnSpeed = 180f;
}

    void FixedUpdate()
    {

    }

        // Update is called once per frame
        void Update () {
		if(Input.GetKeyDown("1"))
            anim.Play("WAIT01", -1, 0f);

        if (Input.GetKeyDown("2"))
            anim.Play("WAIT02", -1, 0f);

        if (Input.GetKeyDown("3"))
            anim.Play("WAIT03", -1, 0f);

        if (Input.GetKeyDown("4"))
            anim.Play("WAIT04", -1, 0f);

        if (Input.GetKeyDown("5"))
            anim.Play("WIN00", -1, -0f);

        //Running
        if (Input.GetKey(KeyCode.LeftShift))
            run = true;
        else
            run = false;

        if (Input.GetKey(KeyCode.Space))
            anim.SetBool("jump", true);
        else
            anim.SetBool("jump", false);

        //Axis setup
        inputH = Input.GetAxis("Horizontal");
        inputV = Input.GetAxis("Vertical");

        //Animation
        anim.SetFloat("inputH", inputH);
        anim.SetFloat("inputV", inputV);
        anim.SetBool("run", run);

        m_MovementInputValue = Input.GetAxis("Vertical");
        m_TurnInputValue = Input.GetAxis("Horizontal");
        Move();
        Turn();
    }

    private void Move()
    {
        // Create a vector in the direction the tank is facing with a magnitude based on the input, speed and the time between frames.
        Vector3 movement = transform.forward * m_MovementInputValue * m_Speed * Time.deltaTime;

        if (run)
            movement *= 3f;

        // Apply this movement to the rigidbody's position.
        rbody.MovePosition(rbody.position + movement);
    }


    private void Turn()
    {
        // Determine the number of degrees to be turned based on the input, speed and time between frames.
        float turn = m_TurnInputValue * m_TurnSpeed * Time.deltaTime;

        // Make this into a rotation in the y axis.
        Quaternion turnRotation = Quaternion.Euler(0f, turn, 0f);

        // Apply this rotation to the rigidbody's rotation.
        rbody.MoveRotation(rbody.rotation * turnRotation);
    }
}
