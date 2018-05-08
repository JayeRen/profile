using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TileManager : MonoBehaviour
{
    public GameObject[] tilePrefabs;

    private Transform playerTransform;
    private float spawnZ = 0.0f;
    private float tileLength = 20.0f;
    private int tileOnScreen = 5;
    private int safeZone = 1;
    private List<GameObject> activeTiles;
    private int lastTileIndex = 0;

    // Use this for initialization
    void Start()
    {
        activeTiles = new List<GameObject>();
        playerTransform = GameObject.FindGameObjectWithTag("Player").transform;
        for (int i = 0; i < safeZone; i++)
        {
            spawnTile(0);
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (playerTransform.position.z > (spawnZ - tileOnScreen * tileLength))
        {
            spawnTile();
        }
        if (activeTiles.Count > 2 * tileOnScreen)
        {
            deleteTile();
        }
    }

    private void spawnTile(int prefabNo = -1)
    {
        GameObject go;
        if (prefabNo == -1)
        {
            go = Instantiate(tilePrefabs[randomTileIndex()]) as GameObject;
        }
        else
        {
            go = Instantiate(tilePrefabs[prefabNo]) as GameObject;
        }

        go.transform.SetParent(transform);
        go.transform.position = Vector3.forward * spawnZ;
        spawnZ += tileLength;
        activeTiles.Add(go);
    }
    private void deleteTile()
    {
        Destroy(activeTiles[0]);
        activeTiles.RemoveAt(0);
    }
    private int randomTileIndex()
    {
        if (tilePrefabs.Length <= 1)
        {
            return 0;
        }
        int randomIndex = lastTileIndex;
        while (randomIndex == lastTileIndex)
        {
            randomIndex = Random.Range(0, tilePrefabs.Length);
        }
        lastTileIndex = randomIndex;
        return randomIndex;
    }
}
