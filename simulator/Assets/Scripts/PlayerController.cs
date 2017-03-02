using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.IO;
using System.Linq;
using System.Text;

public class PlayerController : MonoBehaviour
{
    public float m_Speed = 2f;
    public float m_TurnSpeed = 180f;
    bool simulating = true;

    private string m_MovementAxisName;          // The name of the input axis for moving forward and back.
    private string m_TurnAxisName;              // The name of the input axis for turning.
    private Rigidbody m_Rigidbody;              // Reference used to move the tank.
    private float m_MovementInputValue;         // The current value of the movement input.
    private float m_TurnInputValue;             // The current value of the turn input.

    // Use this for initialization
    void Start () {
        m_Rigidbody = GetComponent<Rigidbody>();

        // When the tank is turned on, make sure it's not kinematic.
        m_Rigidbody.isKinematic = false;

        // Also reset the input values.
        m_MovementInputValue = 0f;
        m_TurnInputValue = 0f;

        m_MovementAxisName = "Vertical";
        m_TurnAxisName = "Horizontal";

    }

    private void Update()
    {
    }

    // Update is called once per frame
    void FixedUpdate ()
    {

        const string input = "D:/command.txt";
        const string output = "D:/camera.png";

        //Starts a new reading cycle
        bool opened = false;
        while (!opened && simulating)
        {
            try
            {
                //Try to get the input from the controller
                StreamReader sr = new StreamReader(input);
                string command = sr.ReadLine();
                string data = sr.ReadToEnd();
                sr.Close();
                File.Delete(input);

                opened = true;      //Stop the reading cycle

                //Reaction the the command of the controller
                if (command == "stop")//Stop simulation
                    simulating = false;
                else
                {
                    if (command == "forward")
                    {
                        m_MovementInputValue = 0.5f;
                        m_TurnInputValue = 0f;
                    }
                    else if(command == "backward")
                    {
                        m_MovementInputValue = -0.5f;
                        m_TurnInputValue = 0f;
                    }
                    else if (command == "right")
                    {
                        m_TurnInputValue = 0.1f;
                        m_MovementInputValue = 0f;
                    }

                    else if (command == "left")
                    {
                        m_TurnInputValue = -0.1f;
                        m_MovementInputValue = 0f;
                    }
                    // Create a texture the size of the screen
                    int width = Screen.width;
                    int height = Screen.height;
                    Texture2D tex = new Texture2D(width, height, TextureFormat.RGB24, false);

                    // Read screen contents into the texture
                    tex.ReadPixels(new Rect(0, 0, width, height), 0, 0);
                    tex.Apply();

                    // Encode texture into PNG
                    byte[] bytes = tex.EncodeToPNG();
                    Destroy(tex);

                    // Write the image
                     File.WriteAllBytes(output, bytes);

                   /* StreamWriter sw = new StreamWriter(output);
                    sw.Write("hello");
                    sw.Close();*/
                }
            }
            catch (Exception e)
            {

            }
        }

        if (!simulating)
        {
            m_MovementInputValue = Input.GetAxis(m_MovementAxisName);
            m_TurnInputValue = Input.GetAxis(m_TurnAxisName);
        }
        Move();
        Turn();
    }

    private void Move()
    {
        // Create a vector in the direction the tank is facing with a magnitude based on the input, speed and the time between frames.
        Vector3 movement;
        if ( simulating)
            movement = transform.forward * m_MovementInputValue * m_Speed;
        else
            movement = transform.forward * m_MovementInputValue * m_Speed * Time.deltaTime;

        // Apply this movement to the rigidbody's position.
        m_Rigidbody.MovePosition(m_Rigidbody.position + movement);
    }


    private void Turn()
    {
        // Determine the number of degrees to be turned based on the input, speed and time between frames.
        float turn;
        if (simulating)
            turn = m_TurnInputValue * m_TurnSpeed;
        else
            turn = m_TurnInputValue * m_TurnSpeed * Time.deltaTime;

        // Make this into a rotation in the y axis.
        Quaternion turnRotation = Quaternion.Euler(0f, turn, 0f);

        // Apply this rotation to the rigidbody's rotation.
        m_Rigidbody.MoveRotation(m_Rigidbody.rotation * turnRotation);
    }
}
