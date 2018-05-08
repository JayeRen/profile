using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PlayerHealth : MonoBehaviour
{

    private int maxHP = 100;
    private int currentHP;
    private bool isDead = false;
    public Slider hpBar;

    private PlayerMotor playerMotor;
    private MonsterMotor monsterMotor;
    public Menu menu;

    // Use this for initialization
    void Start()
    {
        playerMotor = GetComponent<PlayerMotor>();
        monsterMotor = GameObject.FindGameObjectWithTag("Monster").GetComponent<MonsterMotor>();
        currentHP = maxHP;
    }

    // Update is called once per frame
    void Update()
    {

    }
    public void takeDamage(int amount)
    {
        currentHP -= amount;
        hpBar.value = currentHP;
        if (currentHP <= 0 & !isDead)
        {
            death();
        }
    }
    private void death()
    {
        isDead = true;
        menu.displayMenu();
        // stop the player and the monster from moving
        playerMotor.enabled = false;
        monsterMotor.enabled = false;
    }
}
