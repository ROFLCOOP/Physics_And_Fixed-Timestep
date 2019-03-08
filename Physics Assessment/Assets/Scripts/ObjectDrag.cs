using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class ObjectDrag : MonoBehaviour {

    float distance;
    private bool clicked = false;
    private void OnMouseDown()
    {
        Destroy(gameObject);
        
        clicked = true;
    }

    private void OnMouseDrag()
    {
        //Debug.Log("box was dragged");
        GameObject camera = GameObject.Find("Main Camera");
        Rigidbody rigidbody = GetComponent<Rigidbody>();
        
    }

    private void OnMouseUp()
    {
        clicked = false;
    }

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
