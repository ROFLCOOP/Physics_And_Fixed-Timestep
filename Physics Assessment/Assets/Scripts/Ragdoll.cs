using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Animator))]
public class Ragdoll : MonoBehaviour {

    // Use this for initialization
    private Animator animator = null;
    public List<Rigidbody> rigidbodies = new List<Rigidbody>();

    public bool RagdollOn
    {
        get { return !animator.enabled; }
        set
        {
            animator.enabled = !value;
            foreach (Rigidbody r in rigidbodies)
                r.isKinematic = !value;
        }
    }
	void Start () {
        animator = GetComponent<Animator>();

        foreach(Transform child in transform)
        {
            Rigidbody r = child.gameObject.GetComponent<Rigidbody>();
            if(r != null)
            {
                rigidbodies.Add(r);
            }
        }

        foreach(Rigidbody r in rigidbodies)
        {
            r.isKinematic = true;
        }
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
